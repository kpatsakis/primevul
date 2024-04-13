static struct sock *__iucv_get_sock_by_name(char *nm)
{
	struct sock *sk;

	sk_for_each(sk, &iucv_sk_list.head)
		if (!memcmp(&iucv_sk(sk)->src_name, nm, 8))
			return sk;

	return NULL;
}
