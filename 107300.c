static int net_fd(struct wif *wi)
{
	struct priv_net *pn = wi_priv(wi);

	return pn->pn_s;
}
