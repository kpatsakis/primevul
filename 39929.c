static void iucv_sock_init(struct sock *sk, struct sock *parent)
{
	if (parent)
		sk->sk_type = parent->sk_type;
}
