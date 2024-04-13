static u64 vmci_transport_get_buffer_size(struct vsock_sock *vsk)
{
	return vmci_trans(vsk)->queue_pair_size;
}
