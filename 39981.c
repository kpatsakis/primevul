static void cfsk_hold(struct cflayer *layr)
{
	struct caifsock *cf_sk = container_of(layr, struct caifsock, layer);
	sock_hold(&cf_sk->sk);
}
