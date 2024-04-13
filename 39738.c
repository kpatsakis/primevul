static u64 vmci_transport_get_max_buffer_size(struct vsock_sock *vsk)
{
	return vmci_trans(vsk)->queue_pair_max_size;
}
