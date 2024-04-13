static int iucv_sock_connect(struct socket *sock, struct sockaddr *addr,
			     int alen, int flags)
{
	struct sockaddr_iucv *sa = (struct sockaddr_iucv *) addr;
	struct sock *sk = sock->sk;
	struct iucv_sock *iucv = iucv_sk(sk);
	int err;

	if (addr->sa_family != AF_IUCV || alen < sizeof(struct sockaddr_iucv))
		return -EINVAL;

	if (sk->sk_state != IUCV_OPEN && sk->sk_state != IUCV_BOUND)
		return -EBADFD;

	if (sk->sk_state == IUCV_OPEN &&
	    iucv->transport == AF_IUCV_TRANS_HIPER)
		return -EBADFD; /* explicit bind required */

	if (sk->sk_type != SOCK_STREAM && sk->sk_type != SOCK_SEQPACKET)
		return -EINVAL;

	if (sk->sk_state == IUCV_OPEN) {
		err = iucv_sock_autobind(sk);
		if (unlikely(err))
			return err;
	}

	lock_sock(sk);

	/* Set the destination information */
	memcpy(iucv->dst_user_id, sa->siucv_user_id, 8);
	memcpy(iucv->dst_name, sa->siucv_name, 8);

	if (iucv->transport == AF_IUCV_TRANS_HIPER)
		err = iucv_send_ctrl(sock->sk, AF_IUCV_FLAG_SYN);
	else
		err = afiucv_path_connect(sock, addr);
	if (err)
		goto done;

	if (sk->sk_state != IUCV_CONNECTED)
		err = iucv_sock_wait(sk, iucv_sock_in_state(sk, IUCV_CONNECTED,
							    IUCV_DISCONN),
				     sock_sndtimeo(sk, flags & O_NONBLOCK));

	if (sk->sk_state == IUCV_DISCONN || sk->sk_state == IUCV_CLOSED)
		err = -ECONNREFUSED;

	if (err && iucv->transport == AF_IUCV_TRANS_IUCV)
		iucv_sever_path(sk, 0);

done:
	release_sock(sk);
	return err;
}
