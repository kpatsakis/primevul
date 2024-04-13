static int afiucv_path_connect(struct socket *sock, struct sockaddr *addr)
{
	struct sockaddr_iucv *sa = (struct sockaddr_iucv *) addr;
	struct sock *sk = sock->sk;
	struct iucv_sock *iucv = iucv_sk(sk);
	unsigned char user_data[16];
	int err;

	high_nmcpy(user_data, sa->siucv_name);
	low_nmcpy(user_data, iucv->src_name);
	ASCEBC(user_data, sizeof(user_data));

	/* Create path. */
	iucv->path = iucv_path_alloc(iucv->msglimit,
				     IUCV_IPRMDATA, GFP_KERNEL);
	if (!iucv->path) {
		err = -ENOMEM;
		goto done;
	}
	err = pr_iucv->path_connect(iucv->path, &af_iucv_handler,
				    sa->siucv_user_id, NULL, user_data,
				    sk);
	if (err) {
		iucv_path_free(iucv->path);
		iucv->path = NULL;
		switch (err) {
		case 0x0b:	/* Target communicator is not logged on */
			err = -ENETUNREACH;
			break;
		case 0x0d:	/* Max connections for this guest exceeded */
		case 0x0e:	/* Max connections for target guest exceeded */
			err = -EAGAIN;
			break;
		case 0x0f:	/* Missing IUCV authorization */
			err = -EACCES;
			break;
		default:
			err = -ECONNREFUSED;
			break;
		}
	}
done:
	return err;
}
