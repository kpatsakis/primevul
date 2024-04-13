static u64 vmci_transport_stream_rcvhiwat(struct vsock_sock *vsk)
{
	return vmci_trans(vsk)->consume_size;
}
