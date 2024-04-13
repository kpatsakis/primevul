static int net_get_monitor(struct wif *wi)
{
	return net_cmd(wi_priv(wi), NET_GET_MONITOR, NULL, 0);
}
