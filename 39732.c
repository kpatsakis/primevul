static int vmci_transport_dgram_bind(struct vsock_sock *vsk,
				     struct sockaddr_vm *addr)
{
	u32 port;
	u32 flags;
	int err;

	/* VMCI will select a resource ID for us if we provide
	 * VMCI_INVALID_ID.
	 */
	port = addr->svm_port == VMADDR_PORT_ANY ?
			VMCI_INVALID_ID : addr->svm_port;

	if (port <= LAST_RESERVED_PORT && !capable(CAP_NET_BIND_SERVICE))
		return -EACCES;

	flags = addr->svm_cid == VMADDR_CID_ANY ?
				VMCI_FLAG_ANYCID_DG_HND : 0;

	err = vmci_transport_datagram_create_hnd(port, flags,
						 vmci_transport_recv_dgram_cb,
						 &vsk->sk,
						 &vmci_trans(vsk)->dg_handle);
	if (err < VMCI_SUCCESS)
		return vmci_transport_error_to_vsock_error(err);
	vsock_addr_init(&vsk->local_addr, addr->svm_cid,
			vmci_trans(vsk)->dg_handle.resource);

	return 0;
}
