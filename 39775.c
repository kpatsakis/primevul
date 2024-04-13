vmci_transport_send_control_pkt(struct sock *sk,
				enum vmci_transport_packet_type type,
				u64 size,
				u64 mode,
				struct vmci_transport_waiting_info *wait,
				u16 proto,
				struct vmci_handle handle)
{
	struct vmci_transport_packet *pkt;
	struct vsock_sock *vsk;
	int err;

	vsk = vsock_sk(sk);

	if (!vsock_addr_bound(&vsk->local_addr))
		return -EINVAL;

	if (!vsock_addr_bound(&vsk->remote_addr))
		return -EINVAL;

	pkt = kmalloc(sizeof(*pkt), GFP_KERNEL);
	if (!pkt)
		return -ENOMEM;

	err = __vmci_transport_send_control_pkt(pkt, &vsk->local_addr,
						&vsk->remote_addr, type, size,
						mode, wait, proto, handle,
						true);
	kfree(pkt);

	return err;
}
