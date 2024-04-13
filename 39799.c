static bool vmci_transport_stream_is_active(struct vsock_sock *vsk)
{
	return !vmci_handle_is_invalid(vmci_trans(vsk)->qp_handle);
}
