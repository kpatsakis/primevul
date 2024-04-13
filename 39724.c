s64 vsock_stream_has_data(struct vsock_sock *vsk)
{
	return transport->stream_has_data(vsk);
}
