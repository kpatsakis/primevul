static int vmci_transport_dgram_enqueue(
	struct vsock_sock *vsk,
	struct sockaddr_vm *remote_addr,
	struct iovec *iov,
	size_t len)
{
	int err;
	struct vmci_datagram *dg;

	if (len > VMCI_MAX_DG_PAYLOAD_SIZE)
		return -EMSGSIZE;

	if (!vmci_transport_allow_dgram(vsk, remote_addr->svm_cid))
		return -EPERM;

	/* Allocate a buffer for the user's message and our packet header. */
	dg = kmalloc(len + sizeof(*dg), GFP_KERNEL);
	if (!dg)
		return -ENOMEM;

	memcpy_fromiovec(VMCI_DG_PAYLOAD(dg), iov, len);

	dg->dst = vmci_make_handle(remote_addr->svm_cid,
				   remote_addr->svm_port);
	dg->src = vmci_make_handle(vsk->local_addr.svm_cid,
				   vsk->local_addr.svm_port);
	dg->payload_size = len;

	err = vmci_datagram_send(dg);
	kfree(dg);
	if (err < 0)
		return vmci_transport_error_to_vsock_error(err);

	return err - sizeof(*dg);
}
