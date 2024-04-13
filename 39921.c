static int iucv_sock_bind(struct socket *sock, struct sockaddr *addr,
			  int addr_len)
{
	struct sockaddr_iucv *sa = (struct sockaddr_iucv *) addr;
	struct sock *sk = sock->sk;
	struct iucv_sock *iucv;
	int err = 0;
	struct net_device *dev;
	char uid[9];

	/* Verify the input sockaddr */
	if (!addr || addr->sa_family != AF_IUCV)
		return -EINVAL;

	lock_sock(sk);
	if (sk->sk_state != IUCV_OPEN) {
		err = -EBADFD;
		goto done;
	}

	write_lock_bh(&iucv_sk_list.lock);

	iucv = iucv_sk(sk);
	if (__iucv_get_sock_by_name(sa->siucv_name)) {
		err = -EADDRINUSE;
		goto done_unlock;
	}
	if (iucv->path)
		goto done_unlock;

	/* Bind the socket */
	if (pr_iucv)
		if (!memcmp(sa->siucv_user_id, iucv_userid, 8))
			goto vm_bind; /* VM IUCV transport */

	/* try hiper transport */
	memcpy(uid, sa->siucv_user_id, sizeof(uid));
	ASCEBC(uid, 8);
	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev) {
		if (!memcmp(dev->perm_addr, uid, 8)) {
			memcpy(iucv->src_name, sa->siucv_name, 8);
			memcpy(iucv->src_user_id, sa->siucv_user_id, 8);
			sk->sk_bound_dev_if = dev->ifindex;
			iucv->hs_dev = dev;
			dev_hold(dev);
			sk->sk_state = IUCV_BOUND;
			iucv->transport = AF_IUCV_TRANS_HIPER;
			if (!iucv->msglimit)
				iucv->msglimit = IUCV_HIPER_MSGLIM_DEFAULT;
			rcu_read_unlock();
			goto done_unlock;
		}
	}
	rcu_read_unlock();
vm_bind:
	if (pr_iucv) {
		/* use local userid for backward compat */
		memcpy(iucv->src_name, sa->siucv_name, 8);
		memcpy(iucv->src_user_id, iucv_userid, 8);
		sk->sk_state = IUCV_BOUND;
		iucv->transport = AF_IUCV_TRANS_IUCV;
		if (!iucv->msglimit)
			iucv->msglimit = IUCV_QUEUELEN_DEFAULT;
		goto done_unlock;
	}
	/* found no dev to bind */
	err = -ENODEV;
done_unlock:
	/* Release the socket list lock */
	write_unlock_bh(&iucv_sk_list.lock);
done:
	release_sock(sk);
	return err;
}
