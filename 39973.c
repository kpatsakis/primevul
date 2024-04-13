static void caif_read_unlock(struct sock *sk)
{
	struct caifsock *cf_sk;
	cf_sk = container_of(sk, struct caifsock, sk);
	mutex_unlock(&cf_sk->readlock);
}
