static int net_get_nopacket(struct priv_net *pn, void *arg, int *len)
{
	unsigned char buf[2048];
	int l = sizeof(buf);
	int c;

	while (1) {
		l = sizeof(buf);
		c = net_get(pn->pn_s, buf, &l);

		if (c != NET_PACKET && c > 0)
			break;

                if(c > 0)
                    net_enque(pn, buf, l);
	}

	assert(l <= *len);
	memcpy(arg, buf, l);
	*len = l;

	return c;
}
