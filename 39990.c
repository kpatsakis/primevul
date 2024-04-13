static int tx_flow_is_on(struct caifsock *cf_sk)
{
	return test_bit(TX_FLOW_ON_BIT,
			(void *) &cf_sk->flow_state);
}
