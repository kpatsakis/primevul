static int net_set_channel(struct wif *wi, int chan)
{
	uint32_t c = htonl(chan);

	return net_cmd(wi_priv(wi), NET_SET_CHAN, &c, sizeof(c));
}
