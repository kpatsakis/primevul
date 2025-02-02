unsigned long sock_i_ino(struct sock *sk)
{
	unsigned long ino;

	read_lock_bh(&sk->sk_callback_lock);
	ino = sk->sk_socket ? SOCK_INODE(sk->sk_socket)->i_ino : 0;
	read_unlock_bh(&sk->sk_callback_lock);
	return ino;
}
