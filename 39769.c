static void vmci_transport_release(struct vsock_sock *vsk)
{
	if (!vmci_handle_is_invalid(vmci_trans(vsk)->dg_handle)) {
		vmci_datagram_destroy_handle(vmci_trans(vsk)->dg_handle);
		vmci_trans(vsk)->dg_handle = VMCI_INVALID_HANDLE;
	}
}
