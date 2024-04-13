static int rx_flow_is_on(struct caifsock *cf_sk)
{
	return test_bit(RX_FLOW_ON_BIT,
			(void *) &cf_sk->flow_state);
}
