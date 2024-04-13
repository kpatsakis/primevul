struct sock *iucv_accept_dequeue(struct sock *parent, struct socket *newsock)
{
	struct iucv_sock *isk, *n;
	struct sock *sk;

	list_for_each_entry_safe(isk, n, &iucv_sk(parent)->accept_q, accept_q) {
		sk = (struct sock *) isk;
		lock_sock(sk);

		if (sk->sk_state == IUCV_CLOSED) {
			iucv_accept_unlink(sk);
			release_sock(sk);
			continue;
		}

		if (sk->sk_state == IUCV_CONNECTED ||
		    sk->sk_state == IUCV_DISCONN ||
		    !newsock) {
			iucv_accept_unlink(sk);
			if (newsock)
				sock_graft(sk, newsock);

			release_sock(sk);
			return sk;
		}

		release_sock(sk);
	}
	return NULL;
}
