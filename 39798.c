static s64 vmci_transport_stream_has_space(struct vsock_sock *vsk)
{
	return vmci_qpair_produce_free_space(vmci_trans(vsk)->qpair);
}
