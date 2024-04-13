static int net_write(struct wif *wi, unsigned char *h80211, int len,
		     struct tx_info *ti)
{
	struct priv_net *pn = wi_priv(wi);
	int sz = sizeof(*ti);
	unsigned char buf[2048];
	unsigned char *ptr = buf;

	/* XXX */
	if (ti)
		memcpy(ptr, ti, sz);
	else
		memset(ptr, 0, sizeof(*ti));

	ptr += sz;
	memcpy(ptr, h80211, len);
	sz += len;

	return net_cmd(pn, NET_WRITE, buf, sz);
}
