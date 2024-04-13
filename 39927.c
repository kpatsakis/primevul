static int iucv_sock_getsockopt(struct socket *sock, int level, int optname,
				char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	struct iucv_sock *iucv = iucv_sk(sk);
	unsigned int val;
	int len;

	if (level != SOL_IUCV)
		return -ENOPROTOOPT;

	if (get_user(len, optlen))
		return -EFAULT;

	if (len < 0)
		return -EINVAL;

	len = min_t(unsigned int, len, sizeof(int));

	switch (optname) {
	case SO_IPRMDATA_MSG:
		val = (iucv->flags & IUCV_IPRMDATA) ? 1 : 0;
		break;
	case SO_MSGLIMIT:
		lock_sock(sk);
		val = (iucv->path != NULL) ? iucv->path->msglim	/* connected */
					   : iucv->msglimit;	/* default */
		release_sock(sk);
		break;
	case SO_MSGSIZE:
		if (sk->sk_state == IUCV_OPEN)
			return -EBADFD;
		val = (iucv->hs_dev) ? iucv->hs_dev->mtu -
				sizeof(struct af_iucv_trans_hdr) - ETH_HLEN :
				0x7fffffff;
		break;
	default:
		return -ENOPROTOOPT;
	}

	if (put_user(len, optlen))
		return -EFAULT;
	if (copy_to_user(optval, &val, len))
		return -EFAULT;

	return 0;
}
