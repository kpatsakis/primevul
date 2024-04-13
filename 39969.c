static void caif_flow_ctrl(struct sock *sk, int mode)
{
	struct caifsock *cf_sk;
	cf_sk = container_of(sk, struct caifsock, sk);
	if (cf_sk->layer.dn && cf_sk->layer.dn->modemcmd)
		cf_sk->layer.dn->modemcmd(cf_sk->layer.dn, mode);
}
