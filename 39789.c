static void vmci_transport_set_buffer_size(struct vsock_sock *vsk, u64 val)
{
	if (val < vmci_trans(vsk)->queue_pair_min_size)
		vmci_trans(vsk)->queue_pair_min_size = val;
	if (val > vmci_trans(vsk)->queue_pair_max_size)
		vmci_trans(vsk)->queue_pair_max_size = val;
	vmci_trans(vsk)->queue_pair_size = val;
}
