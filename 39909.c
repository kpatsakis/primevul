static void iucv_callback_connrej(struct iucv_path *path, u8 ipuser[16])
{
	struct sock *sk = path->private;

	if (sk->sk_state == IUCV_CLOSED)
		return;

	bh_lock_sock(sk);
	iucv_sever_path(sk, 1);
	sk->sk_state = IUCV_DISCONN;

	sk->sk_state_change(sk);
	bh_unlock_sock(sk);
}
