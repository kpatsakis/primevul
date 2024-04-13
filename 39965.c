static void caif_check_flow_release(struct sock *sk)
{
	struct caifsock *cf_sk = container_of(sk, struct caifsock, sk);

	if (rx_flow_is_on(cf_sk))
		return;

	if (atomic_read(&sk->sk_rmem_alloc) <= sk_rcvbuf_lowwater(cf_sk)) {
			set_rx_flow_on(cf_sk);
			caif_flow_ctrl(sk, CAIF_MODEMCMD_FLOW_ON_REQ);
	}
}
