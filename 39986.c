static void set_tx_flow_off(struct caifsock *cf_sk)
{
	 clear_bit(TX_FLOW_ON_BIT,
		(void *) &cf_sk->flow_state);
}
