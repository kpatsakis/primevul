static void vmci_transport_set_min_buffer_size(struct vsock_sock *vsk,
					       u64 val)
{
	if (val > vmci_trans(vsk)->queue_pair_size)
		vmci_trans(vsk)->queue_pair_size = val;
	vmci_trans(vsk)->queue_pair_min_size = val;
}
