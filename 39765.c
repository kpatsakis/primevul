vmci_transport_recv_connecting_server(struct sock *listener,
				      struct sock *pending,
				      struct vmci_transport_packet *pkt)
{
	struct vsock_sock *vpending;
	struct vmci_handle handle;
	struct vmci_qp *qpair;
	bool is_local;
	u32 flags;
	u32 detach_sub_id;
	int err;
	int skerr;

	vpending = vsock_sk(pending);
	detach_sub_id = VMCI_INVALID_ID;

	switch (pkt->type) {
	case VMCI_TRANSPORT_PACKET_TYPE_OFFER:
		if (vmci_handle_is_invalid(pkt->u.handle)) {
			vmci_transport_send_reset(pending, pkt);
			skerr = EPROTO;
			err = -EINVAL;
			goto destroy;
		}
		break;
	default:
		/* Close and cleanup the connection. */
		vmci_transport_send_reset(pending, pkt);
		skerr = EPROTO;
		err = pkt->type == VMCI_TRANSPORT_PACKET_TYPE_RST ? 0 : -EINVAL;
		goto destroy;
	}

	/* In order to complete the connection we need to attach to the offered
	 * queue pair and send an attach notification.  We also subscribe to the
	 * detach event so we know when our peer goes away, and we do that
	 * before attaching so we don't miss an event.  If all this succeeds,
	 * we update our state and wakeup anything waiting in accept() for a
	 * connection.
	 */

	/* We don't care about attach since we ensure the other side has
	 * attached by specifying the ATTACH_ONLY flag below.
	 */
	err = vmci_event_subscribe(VMCI_EVENT_QP_PEER_DETACH,
				   vmci_transport_peer_detach_cb,
				   pending, &detach_sub_id);
	if (err < VMCI_SUCCESS) {
		vmci_transport_send_reset(pending, pkt);
		err = vmci_transport_error_to_vsock_error(err);
		skerr = -err;
		goto destroy;
	}

	vmci_trans(vpending)->detach_sub_id = detach_sub_id;

	/* Now attach to the queue pair the client created. */
	handle = pkt->u.handle;

	/* vpending->local_addr always has a context id so we do not need to
	 * worry about VMADDR_CID_ANY in this case.
	 */
	is_local =
	    vpending->remote_addr.svm_cid == vpending->local_addr.svm_cid;
	flags = VMCI_QPFLAG_ATTACH_ONLY;
	flags |= is_local ? VMCI_QPFLAG_LOCAL : 0;

	err = vmci_transport_queue_pair_alloc(
					&qpair,
					&handle,
					vmci_trans(vpending)->produce_size,
					vmci_trans(vpending)->consume_size,
					pkt->dg.src.context,
					flags,
					vmci_transport_is_trusted(
						vpending,
						vpending->remote_addr.svm_cid));
	if (err < 0) {
		vmci_transport_send_reset(pending, pkt);
		skerr = -err;
		goto destroy;
	}

	vmci_trans(vpending)->qp_handle = handle;
	vmci_trans(vpending)->qpair = qpair;

	/* When we send the attach message, we must be ready to handle incoming
	 * control messages on the newly connected socket. So we move the
	 * pending socket to the connected state before sending the attach
	 * message. Otherwise, an incoming packet triggered by the attach being
	 * received by the peer may be processed concurrently with what happens
	 * below after sending the attach message, and that incoming packet
	 * will find the listening socket instead of the (currently) pending
	 * socket. Note that enqueueing the socket increments the reference
	 * count, so even if a reset comes before the connection is accepted,
	 * the socket will be valid until it is removed from the queue.
	 *
	 * If we fail sending the attach below, we remove the socket from the
	 * connected list and move the socket to SS_UNCONNECTED before
	 * releasing the lock, so a pending slow path processing of an incoming
	 * packet will not see the socket in the connected state in that case.
	 */
	pending->sk_state = SS_CONNECTED;

	vsock_insert_connected(vpending);

	/* Notify our peer of our attach. */
	err = vmci_transport_send_attach(pending, handle);
	if (err < 0) {
		vsock_remove_connected(vpending);
		pr_err("Could not send attach\n");
		vmci_transport_send_reset(pending, pkt);
		err = vmci_transport_error_to_vsock_error(err);
		skerr = -err;
		goto destroy;
	}

	/* We have a connection. Move the now connected socket from the
	 * listener's pending list to the accept queue so callers of accept()
	 * can find it.
	 */
	vsock_remove_pending(listener, pending);
	vsock_enqueue_accept(listener, pending);

	/* Callers of accept() will be be waiting on the listening socket, not
	 * the pending socket.
	 */
	listener->sk_state_change(listener);

	return 0;

destroy:
	pending->sk_err = skerr;
	pending->sk_state = SS_UNCONNECTED;
	/* As long as we drop our reference, all necessary cleanup will handle
	 * when the cleanup function drops its reference and our destruct
	 * implementation is called.  Note that since the listen handler will
	 * remove pending from the pending list upon our failure, the cleanup
	 * function won't drop the additional reference, which is why we do it
	 * here.
	 */
	sock_put(pending);

	return err;
}
