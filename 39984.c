static void set_rx_flow_off(struct caifsock *cf_sk)
{
	 clear_bit(RX_FLOW_ON_BIT,
		 (void *) &cf_sk->flow_state);
}
