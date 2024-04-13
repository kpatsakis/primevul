dtls1_min_mtu(void)
	{
	return (g_probable_mtu[(sizeof(g_probable_mtu) / 
		sizeof(g_probable_mtu[0])) - 1]);
	}
