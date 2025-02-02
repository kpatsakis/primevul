static int iucv_sock_shutdown(struct socket *sock, int how)
{
	struct sock *sk = sock->sk;
	struct iucv_sock *iucv = iucv_sk(sk);
	struct iucv_message txmsg;
	int err = 0;

	how++;

	if ((how & ~SHUTDOWN_MASK) || !how)
		return -EINVAL;

	lock_sock(sk);
	switch (sk->sk_state) {
	case IUCV_LISTEN:
	case IUCV_DISCONN:
	case IUCV_CLOSING:
	case IUCV_CLOSED:
		err = -ENOTCONN;
		goto fail;
	default:
		break;
	}

	if (how == SEND_SHUTDOWN || how == SHUTDOWN_MASK) {
		if (iucv->transport == AF_IUCV_TRANS_IUCV) {
			txmsg.class = 0;
			txmsg.tag = 0;
			err = pr_iucv->message_send(iucv->path, &txmsg,
				IUCV_IPRMDATA, 0, (void *) iprm_shutdown, 8);
			if (err) {
				switch (err) {
				case 1:
					err = -ENOTCONN;
					break;
				case 2:
					err = -ECONNRESET;
					break;
				default:
					err = -ENOTCONN;
					break;
				}
			}
		} else
			iucv_send_ctrl(sk, AF_IUCV_FLAG_SHT);
	}

	sk->sk_shutdown |= how;
	if (how == RCV_SHUTDOWN || how == SHUTDOWN_MASK) {
		if (iucv->transport == AF_IUCV_TRANS_IUCV) {
			err = pr_iucv->path_quiesce(iucv->path, NULL);
			if (err)
				err = -ENOTCONN;
/*			skb_queue_purge(&sk->sk_receive_queue); */
		}
		skb_queue_purge(&sk->sk_receive_queue);
	}

	/* Wake up anyone sleeping in poll */
	sk->sk_state_change(sk);

fail:
	release_sock(sk);
	return err;
}
