static int vmci_transport_recv_connecting_client_negotiate(
					struct sock *sk,
					struct vmci_transport_packet *pkt)
{
	int err;
	struct vsock_sock *vsk;
	struct vmci_handle handle;
	struct vmci_qp *qpair;
	u32 attach_sub_id;
	u32 detach_sub_id;
	bool is_local;
	u32 flags;
	bool old_proto = true;
	bool old_pkt_proto;
	u16 version;

	vsk = vsock_sk(sk);
	handle = VMCI_INVALID_HANDLE;
	attach_sub_id = VMCI_INVALID_ID;
	detach_sub_id = VMCI_INVALID_ID;

	/* If we have gotten here then we should be past the point where old
	 * linux vsock could have sent the bogus rst.
	 */
	vsk->sent_request = false;
	vsk->ignore_connecting_rst = false;

	/* Verify that we're OK with the proposed queue pair size */
	if (pkt->u.size < vmci_trans(vsk)->queue_pair_min_size ||
	    pkt->u.size > vmci_trans(vsk)->queue_pair_max_size) {
		err = -EINVAL;
		goto destroy;
	}

	/* At this point we know the CID the peer is using to talk to us. */

	if (vsk->local_addr.svm_cid == VMADDR_CID_ANY)
		vsk->local_addr.svm_cid = pkt->dg.dst.context;

	/* Setup the notify ops to be the highest supported version that both
	 * the server and the client support.
	 */

	if (vmci_transport_old_proto_override(&old_pkt_proto)) {
		old_proto = old_pkt_proto;
	} else {
		if (pkt->type == VMCI_TRANSPORT_PACKET_TYPE_NEGOTIATE)
			old_proto = true;
		else if (pkt->type == VMCI_TRANSPORT_PACKET_TYPE_NEGOTIATE2)
			old_proto = false;

	}

	if (old_proto)
		version = VSOCK_PROTO_INVALID;
	else
		version = pkt->proto;

	if (!vmci_transport_proto_to_notify_struct(sk, &version, old_proto)) {
		err = -EINVAL;
		goto destroy;
	}

	/* Subscribe to attach and detach events first.
	 *
	 * XXX We attach once for each queue pair created for now so it is easy
	 * to find the socket (it's provided), but later we should only
	 * subscribe once and add a way to lookup sockets by queue pair handle.
	 */
	err = vmci_event_subscribe(VMCI_EVENT_QP_PEER_ATTACH,
				   vmci_transport_peer_attach_cb,
				   sk, &attach_sub_id);
	if (err < VMCI_SUCCESS) {
		err = vmci_transport_error_to_vsock_error(err);
		goto destroy;
	}

	err = vmci_event_subscribe(VMCI_EVENT_QP_PEER_DETACH,
				   vmci_transport_peer_detach_cb,
				   sk, &detach_sub_id);
	if (err < VMCI_SUCCESS) {
		err = vmci_transport_error_to_vsock_error(err);
		goto destroy;
	}

	/* Make VMCI select the handle for us. */
	handle = VMCI_INVALID_HANDLE;
	is_local = vsk->remote_addr.svm_cid == vsk->local_addr.svm_cid;
	flags = is_local ? VMCI_QPFLAG_LOCAL : 0;

	err = vmci_transport_queue_pair_alloc(&qpair,
					      &handle,
					      pkt->u.size,
					      pkt->u.size,
					      vsk->remote_addr.svm_cid,
					      flags,
					      vmci_transport_is_trusted(
						  vsk,
						  vsk->
						  remote_addr.svm_cid));
	if (err < 0)
		goto destroy;

	err = vmci_transport_send_qp_offer(sk, handle);
	if (err < 0) {
		err = vmci_transport_error_to_vsock_error(err);
		goto destroy;
	}

	vmci_trans(vsk)->qp_handle = handle;
	vmci_trans(vsk)->qpair = qpair;

	vmci_trans(vsk)->produce_size = vmci_trans(vsk)->consume_size =
		pkt->u.size;

	vmci_trans(vsk)->attach_sub_id = attach_sub_id;
	vmci_trans(vsk)->detach_sub_id = detach_sub_id;

	vmci_trans(vsk)->notify_ops->process_negotiate(sk);

	return 0;

destroy:
	if (attach_sub_id != VMCI_INVALID_ID)
		vmci_event_unsubscribe(attach_sub_id);

	if (detach_sub_id != VMCI_INVALID_ID)
		vmci_event_unsubscribe(detach_sub_id);

	if (!vmci_handle_is_invalid(handle))
		vmci_qpair_detach(&qpair);

	return err;
}
