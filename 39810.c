static inline unsigned int llcp_accept_poll(struct sock *parent)
{
	struct nfc_llcp_sock *llcp_sock, *n, *parent_sock;
	struct sock *sk;

	parent_sock = nfc_llcp_sock(parent);

	list_for_each_entry_safe(llcp_sock, n, &parent_sock->accept_queue,
				 accept_queue) {
		sk = &llcp_sock->sk;

		if (sk->sk_state == LLCP_CONNECTED)
			return POLLIN | POLLRDNORM;
	}

	return 0;
}
