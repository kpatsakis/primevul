static s64 vmci_transport_stream_has_data(struct vsock_sock *vsk)
{
	return vmci_qpair_consume_buf_ready(vmci_trans(vsk)->qpair);
}
