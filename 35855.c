static void sock_disable_timestamp(struct sock *sk, unsigned long flags)
{
	if (sk->sk_flags & flags) {
		sk->sk_flags &= ~flags;
		if (!(sk->sk_flags & SK_FLAGS_TIMESTAMP))
			net_disable_timestamp();
	}
}
