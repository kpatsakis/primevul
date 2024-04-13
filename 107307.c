static int net_read(struct wif *wi, unsigned char *h80211, int len,
		    struct rx_info *ri)
{
	struct priv_net *pn = wi_priv(wi);
	uint32_t buf[512]; // 512 * 4 = 2048
	unsigned char *bufc = (unsigned char*)buf;
	int cmd;
	int sz = sizeof(*ri);
	int l;
	int ret;

	/* try queue */
	l = queue_get(pn, buf, sizeof(buf));
	if (!l) {
		/* try reading form net */
		l = sizeof(buf);
		cmd = net_get(pn->pn_s, buf, &l);

		if (cmd == -1)
			return -1;
		if (cmd == NET_RC)
		{
			ret = ntohl((buf[0]));
			return ret;
		}
		assert(cmd == NET_PACKET);
	}

	/* XXX */
	if (ri) {
		ri->ri_mactime = __be64_to_cpu(((uint64_t)buf[0] << 32 || buf[1] ));
		ri->ri_power = __be32_to_cpu(buf[2]);
		ri->ri_noise = __be32_to_cpu(buf[3]);
		ri->ri_channel = __be32_to_cpu(buf[4]);
		ri->ri_rate = __be32_to_cpu(buf[5]);
		ri->ri_antenna = __be32_to_cpu(buf[6]);
	}
	l -= sz;
	assert(l > 0);
	if (l > len)
		l = len;
	memcpy(h80211, &bufc[sz], l);

	return l;
}
