static void net_close(struct wif *wi)
{
	struct priv_net *pn = wi_priv(wi);

	close(pn->pn_s);
	do_net_free(wi);
}
