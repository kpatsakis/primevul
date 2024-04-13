static int sk_rcvbuf_lowwater(struct caifsock *cf_sk)
{
	/* A quarter of full buffer is used a low water mark */
	return cf_sk->sk.sk_rcvbuf / 4;
}
