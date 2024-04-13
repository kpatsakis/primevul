static int caif_connect(struct socket *sock, struct sockaddr *uaddr,
			int addr_len, int flags)
{
	struct sock *sk = sock->sk;
	struct caifsock *cf_sk = container_of(sk, struct caifsock, sk);
	long timeo;
	int err;
	int ifindex, headroom, tailroom;
	unsigned int mtu;
	struct net_device *dev;

	lock_sock(sk);

	err = -EAFNOSUPPORT;
	if (uaddr->sa_family != AF_CAIF)
		goto out;

	switch (sock->state) {
	case SS_UNCONNECTED:
		/* Normal case, a fresh connect */
		caif_assert(sk->sk_state == CAIF_DISCONNECTED);
		break;
	case SS_CONNECTING:
		switch (sk->sk_state) {
		case CAIF_CONNECTED:
			sock->state = SS_CONNECTED;
			err = -EISCONN;
			goto out;
		case CAIF_DISCONNECTED:
			/* Reconnect allowed */
			break;
		case CAIF_CONNECTING:
			err = -EALREADY;
			if (flags & O_NONBLOCK)
				goto out;
			goto wait_connect;
		}
		break;
	case SS_CONNECTED:
		caif_assert(sk->sk_state == CAIF_CONNECTED ||
				sk->sk_state == CAIF_DISCONNECTED);
		if (sk->sk_shutdown & SHUTDOWN_MASK) {
			/* Allow re-connect after SHUTDOWN_IND */
			caif_disconnect_client(sock_net(sk), &cf_sk->layer);
			caif_free_client(&cf_sk->layer);
			break;
		}
		/* No reconnect on a seqpacket socket */
		err = -EISCONN;
		goto out;
	case SS_DISCONNECTING:
	case SS_FREE:
		caif_assert(1); /*Should never happen */
		break;
	}
	sk->sk_state = CAIF_DISCONNECTED;
	sock->state = SS_UNCONNECTED;
	sk_stream_kill_queues(&cf_sk->sk);

	err = -EINVAL;
	if (addr_len != sizeof(struct sockaddr_caif))
		goto out;

	memcpy(&cf_sk->conn_req.sockaddr, uaddr,
		sizeof(struct sockaddr_caif));

	/* Move to connecting socket, start sending Connect Requests */
	sock->state = SS_CONNECTING;
	sk->sk_state = CAIF_CONNECTING;

	/* Check priority value comming from socket */
	/* if priority value is out of range it will be ajusted */
	if (cf_sk->sk.sk_priority > CAIF_PRIO_MAX)
		cf_sk->conn_req.priority = CAIF_PRIO_MAX;
	else if (cf_sk->sk.sk_priority < CAIF_PRIO_MIN)
		cf_sk->conn_req.priority = CAIF_PRIO_MIN;
	else
		cf_sk->conn_req.priority = cf_sk->sk.sk_priority;

	/*ifindex = id of the interface.*/
	cf_sk->conn_req.ifindex = cf_sk->sk.sk_bound_dev_if;

	cf_sk->layer.receive = caif_sktrecv_cb;

	err = caif_connect_client(sock_net(sk), &cf_sk->conn_req,
				&cf_sk->layer, &ifindex, &headroom, &tailroom);

	if (err < 0) {
		cf_sk->sk.sk_socket->state = SS_UNCONNECTED;
		cf_sk->sk.sk_state = CAIF_DISCONNECTED;
		goto out;
	}

	err = -ENODEV;
	rcu_read_lock();
	dev = dev_get_by_index_rcu(sock_net(sk), ifindex);
	if (!dev) {
		rcu_read_unlock();
		goto out;
	}
	cf_sk->headroom = LL_RESERVED_SPACE_EXTRA(dev, headroom);
	mtu = dev->mtu;
	rcu_read_unlock();

	cf_sk->tailroom = tailroom;
	cf_sk->maxframe = mtu - (headroom + tailroom);
	if (cf_sk->maxframe < 1) {
		pr_warn("CAIF Interface MTU too small (%d)\n", dev->mtu);
		err = -ENODEV;
		goto out;
	}

	err = -EINPROGRESS;
wait_connect:

	if (sk->sk_state != CAIF_CONNECTED && (flags & O_NONBLOCK))
		goto out;

	timeo = sock_sndtimeo(sk, flags & O_NONBLOCK);

	release_sock(sk);
	err = -ERESTARTSYS;
	timeo = wait_event_interruptible_timeout(*sk_sleep(sk),
			sk->sk_state != CAIF_CONNECTING,
			timeo);
	lock_sock(sk);
	if (timeo < 0)
		goto out; /* -ERESTARTSYS */

	err = -ETIMEDOUT;
	if (timeo == 0 && sk->sk_state != CAIF_CONNECTED)
		goto out;
	if (sk->sk_state != CAIF_CONNECTED) {
		sock->state = SS_UNCONNECTED;
		err = sock_error(sk);
		if (!err)
			err = -ECONNREFUSED;
		goto out;
	}
	sock->state = SS_CONNECTED;
	err = 0;
out:
	release_sock(sk);
	return err;
}
