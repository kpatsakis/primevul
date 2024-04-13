static ssize_t vmci_transport_stream_dequeue(
	struct vsock_sock *vsk,
	struct iovec *iov,
	size_t len,
	int flags)
{
	if (flags & MSG_PEEK)
		return vmci_qpair_peekv(vmci_trans(vsk)->qpair, iov, len, 0);
	else
		return vmci_qpair_dequev(vmci_trans(vsk)->qpair, iov, len, 0);
}
