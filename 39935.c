static int iucv_sock_setsockopt(struct socket *sock, int level, int optname,
				char __user *optval, unsigned int optlen)
{
	struct sock *sk = sock->sk;
	struct iucv_sock *iucv = iucv_sk(sk);
	int val;
	int rc;

	if (level != SOL_IUCV)
		return -ENOPROTOOPT;

	if (optlen < sizeof(int))
		return -EINVAL;

	if (get_user(val, (int __user *) optval))
		return -EFAULT;

	rc = 0;

	lock_sock(sk);
	switch (optname) {
	case SO_IPRMDATA_MSG:
		if (val)
			iucv->flags |= IUCV_IPRMDATA;
		else
			iucv->flags &= ~IUCV_IPRMDATA;
		break;
	case SO_MSGLIMIT:
		switch (sk->sk_state) {
		case IUCV_OPEN:
		case IUCV_BOUND:
			if (val < 1 || val > (u16)(~0))
				rc = -EINVAL;
			else
				iucv->msglimit = val;
			break;
		default:
			rc = -EINVAL;
			break;
		}
		break;
	default:
		rc = -ENOPROTOOPT;
		break;
	}
	release_sock(sk);

	return rc;
}
