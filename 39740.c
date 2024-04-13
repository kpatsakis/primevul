static struct sock *vmci_transport_get_pending(
					struct sock *listener,
					struct vmci_transport_packet *pkt)
{
	struct vsock_sock *vlistener;
	struct vsock_sock *vpending;
	struct sock *pending;
	struct sockaddr_vm src;

	vsock_addr_init(&src, pkt->dg.src.context, pkt->src_port);

	vlistener = vsock_sk(listener);

	list_for_each_entry(vpending, &vlistener->pending_links,
			    pending_links) {
		if (vsock_addr_equals_addr(&src, &vpending->remote_addr) &&
		    pkt->dst_port == vpending->local_addr.svm_port) {
			pending = sk_vsock(vpending);
			sock_hold(pending);
			goto found;
		}
	}

	pending = NULL;
found:
	return pending;

}
