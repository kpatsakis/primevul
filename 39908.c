static void iucv_callback_connack(struct iucv_path *path, u8 ipuser[16])
{
	struct sock *sk = path->private;

	sk->sk_state = IUCV_CONNECTED;
	sk->sk_state_change(sk);
}
