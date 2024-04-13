struct sock *vsock_find_connected_socket(struct sockaddr_vm *src,
					 struct sockaddr_vm *dst)
{
	struct sock *sk;

	spin_lock_bh(&vsock_table_lock);
	sk = __vsock_find_connected_socket(src, dst);
	if (sk)
		sock_hold(sk);

	spin_unlock_bh(&vsock_table_lock);

	return sk;
}
