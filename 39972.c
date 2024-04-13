static void caif_read_lock(struct sock *sk)
{
	struct caifsock *cf_sk;
	cf_sk = container_of(sk, struct caifsock, sk);
	mutex_lock(&cf_sk->readlock);
}
