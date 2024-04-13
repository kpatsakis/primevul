static int iucv_sock_autobind(struct sock *sk)
{
	struct iucv_sock *iucv = iucv_sk(sk);
	char name[12];
	int err = 0;

	if (unlikely(!pr_iucv))
		return -EPROTO;

	memcpy(iucv->src_user_id, iucv_userid, 8);

	write_lock_bh(&iucv_sk_list.lock);

	sprintf(name, "%08x", atomic_inc_return(&iucv_sk_list.autobind_name));
	while (__iucv_get_sock_by_name(name)) {
		sprintf(name, "%08x",
			atomic_inc_return(&iucv_sk_list.autobind_name));
	}

	write_unlock_bh(&iucv_sk_list.lock);

	memcpy(&iucv->src_name, name, 8);

	if (!iucv->msglimit)
		iucv->msglimit = IUCV_QUEUELEN_DEFAULT;

	return err;
}
