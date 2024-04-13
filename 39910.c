static int iucv_callback_connreq(struct iucv_path *path,
				 u8 ipvmid[8], u8 ipuser[16])
{
	unsigned char user_data[16];
	unsigned char nuser_data[16];
	unsigned char src_name[8];
	struct sock *sk, *nsk;
	struct iucv_sock *iucv, *niucv;
	int err;

	memcpy(src_name, ipuser, 8);
	EBCASC(src_name, 8);
	/* Find out if this path belongs to af_iucv. */
	read_lock(&iucv_sk_list.lock);
	iucv = NULL;
	sk = NULL;
	sk_for_each(sk, &iucv_sk_list.head)
		if (sk->sk_state == IUCV_LISTEN &&
		    !memcmp(&iucv_sk(sk)->src_name, src_name, 8)) {
			/*
			 * Found a listening socket with
			 * src_name == ipuser[0-7].
			 */
			iucv = iucv_sk(sk);
			break;
		}
	read_unlock(&iucv_sk_list.lock);
	if (!iucv)
		/* No socket found, not one of our paths. */
		return -EINVAL;

	bh_lock_sock(sk);

	/* Check if parent socket is listening */
	low_nmcpy(user_data, iucv->src_name);
	high_nmcpy(user_data, iucv->dst_name);
	ASCEBC(user_data, sizeof(user_data));
	if (sk->sk_state != IUCV_LISTEN) {
		err = pr_iucv->path_sever(path, user_data);
		iucv_path_free(path);
		goto fail;
	}

	/* Check for backlog size */
	if (sk_acceptq_is_full(sk)) {
		err = pr_iucv->path_sever(path, user_data);
		iucv_path_free(path);
		goto fail;
	}

	/* Create the new socket */
	nsk = iucv_sock_alloc(NULL, sk->sk_type, GFP_ATOMIC);
	if (!nsk) {
		err = pr_iucv->path_sever(path, user_data);
		iucv_path_free(path);
		goto fail;
	}

	niucv = iucv_sk(nsk);
	iucv_sock_init(nsk, sk);

	/* Set the new iucv_sock */
	memcpy(niucv->dst_name, ipuser + 8, 8);
	EBCASC(niucv->dst_name, 8);
	memcpy(niucv->dst_user_id, ipvmid, 8);
	memcpy(niucv->src_name, iucv->src_name, 8);
	memcpy(niucv->src_user_id, iucv->src_user_id, 8);
	niucv->path = path;

	/* Call iucv_accept */
	high_nmcpy(nuser_data, ipuser + 8);
	memcpy(nuser_data + 8, niucv->src_name, 8);
	ASCEBC(nuser_data + 8, 8);

	/* set message limit for path based on msglimit of accepting socket */
	niucv->msglimit = iucv->msglimit;
	path->msglim = iucv->msglimit;
	err = pr_iucv->path_accept(path, &af_iucv_handler, nuser_data, nsk);
	if (err) {
		iucv_sever_path(nsk, 1);
		iucv_sock_kill(nsk);
		goto fail;
	}

	iucv_accept_enqueue(sk, nsk);

	/* Wake up accept */
	nsk->sk_state = IUCV_CONNECTED;
	sk->sk_data_ready(sk, 1);
	err = 0;
fail:
	bh_unlock_sock(sk);
	return 0;
}
