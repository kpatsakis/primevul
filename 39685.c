static bool __vsock_in_connected_table(struct vsock_sock *vsk)
{
	return !list_empty(&vsk->connected_table);
}
