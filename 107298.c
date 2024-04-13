static int net_cmd(struct priv_net *pn, int command, void *arg, int alen)
{
	uint32_t rc;
	int len;
	int cmd;

	if (net_send(pn->pn_s, command, arg, alen) == -1)
        {
		return -1;
        }

	len = sizeof(rc);
	cmd = net_get_nopacket(pn, &rc, &len);
	if (cmd == -1)
        {
		return -1;
        }
	assert(cmd == NET_RC);
	assert(len == sizeof(rc));

	return ntohl(rc);
}
