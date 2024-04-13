static ssize_t vmci_transport_stream_enqueue(
	struct vsock_sock *vsk,
	struct iovec *iov,
	size_t len)
{
	return vmci_qpair_enquev(vmci_trans(vsk)->qpair, iov, len, 0);
}
