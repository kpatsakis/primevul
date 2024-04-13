static int iucv_sock_in_state(struct sock *sk, int state, int state2)
{
	return (sk->sk_state == state || sk->sk_state == state2);
}
