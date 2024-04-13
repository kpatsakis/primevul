static inline unsigned int iucv_accept_poll(struct sock *parent)
{
	struct iucv_sock *isk, *n;
	struct sock *sk;

	list_for_each_entry_safe(isk, n, &iucv_sk(parent)->accept_q, accept_q) {
		sk = (struct sock *) isk;

		if (sk->sk_state == IUCV_CONNECTED)
			return POLLIN | POLLRDNORM;
	}

	return 0;
}
