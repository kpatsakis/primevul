static void iucv_callback_shutdown(struct iucv_path *path, u8 ipuser[16])
{
	struct sock *sk = path->private;

	bh_lock_sock(sk);
	if (sk->sk_state != IUCV_CLOSED) {
		sk->sk_shutdown |= SEND_SHUTDOWN;
		sk->sk_state_change(sk);
	}
	bh_unlock_sock(sk);
}
