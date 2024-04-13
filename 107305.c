static int net_get_rate(struct wif *wi)
{
	struct priv_net *pn = wi_priv(wi);

	return net_cmd(pn, NET_GET_RATE, NULL, 0);
}
