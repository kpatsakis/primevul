static int net_set_rate(struct wif *wi, int rate)
{
	uint32_t c = htonl(rate);

	return net_cmd(wi_priv(wi), NET_SET_RATE, &c, sizeof(c));
}
