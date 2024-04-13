static int net_get_mac(struct wif *wi, unsigned char *mac)
{
	struct priv_net *pn = wi_priv(wi);
	uint32_t buf[2]; // only need 6 bytes, this provides 8
	int cmd;
	int sz = 6;

	if (net_send(pn->pn_s, NET_GET_MAC, NULL, 0) == -1)
		return -1;

	cmd = net_get_nopacket(pn, buf, &sz);
	if (cmd == -1)
		return -1;
	if (cmd == NET_RC)
		return ntohl(buf[0]);
	assert(cmd == NET_MAC);
	assert(sz == 6);

	memcpy(mac, buf, 6);

	return 0;
}
