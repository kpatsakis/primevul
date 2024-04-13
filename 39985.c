static void set_rx_flow_on(struct caifsock *cf_sk)
{
	 set_bit(RX_FLOW_ON_BIT,
			(void *) &cf_sk->flow_state);
}
