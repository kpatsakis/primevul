static void cfsk_put(struct cflayer *layr)
{
	struct caifsock *cf_sk = container_of(layr, struct caifsock, layer);
	sock_put(&cf_sk->sk);
}
