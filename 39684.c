static bool __vsock_in_bound_table(struct vsock_sock *vsk)
{
	return !list_empty(&vsk->bound_table);
}
