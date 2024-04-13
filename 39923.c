static void iucv_sock_close(struct sock *sk)
{
	struct iucv_sock *iucv = iucv_sk(sk);
	unsigned long timeo;
	int err = 0;

	lock_sock(sk);

	switch (sk->sk_state) {
	case IUCV_LISTEN:
		iucv_sock_cleanup_listen(sk);
		break;

	case IUCV_CONNECTED:
		if (iucv->transport == AF_IUCV_TRANS_HIPER) {
			err = iucv_send_ctrl(sk, AF_IUCV_FLAG_FIN);
			sk->sk_state = IUCV_DISCONN;
			sk->sk_state_change(sk);
		}
	case IUCV_DISCONN:   /* fall through */
		sk->sk_state = IUCV_CLOSING;
		sk->sk_state_change(sk);

		if (!err && !skb_queue_empty(&iucv->send_skb_q)) {
			if (sock_flag(sk, SOCK_LINGER) && sk->sk_lingertime)
				timeo = sk->sk_lingertime;
			else
				timeo = IUCV_DISCONN_TIMEOUT;
			iucv_sock_wait(sk,
					iucv_sock_in_state(sk, IUCV_CLOSED, 0),
					timeo);
		}

	case IUCV_CLOSING:   /* fall through */
		sk->sk_state = IUCV_CLOSED;
		sk->sk_state_change(sk);

		sk->sk_err = ECONNRESET;
		sk->sk_state_change(sk);

		skb_queue_purge(&iucv->send_skb_q);
		skb_queue_purge(&iucv->backlog_skb_q);

	default:   /* fall through */
		iucv_sever_path(sk, 1);
	}

	if (iucv->hs_dev) {
		dev_put(iucv->hs_dev);
		iucv->hs_dev = NULL;
		sk->sk_bound_dev_if = 0;
	}

	/* mark socket for deletion by iucv_sock_kill() */
	sock_set_flag(sk, SOCK_ZAPPED);

	release_sock(sk);
}
