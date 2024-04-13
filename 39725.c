s64 vsock_stream_has_space(struct vsock_sock *vsk)
{
	return transport->stream_has_space(vsk);
}
