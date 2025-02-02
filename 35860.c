int sock_i_uid(struct sock *sk)
{
	int uid;

	read_lock_bh(&sk->sk_callback_lock);
	uid = sk->sk_socket ? SOCK_INODE(sk->sk_socket)->i_uid : 0;
	read_unlock_bh(&sk->sk_callback_lock);
	return uid;
}
