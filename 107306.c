struct wif *net_open(char *iface)
{
	struct wif *wi;
	struct priv_net *pn;
	int s;

	/* setup wi struct */
	wi = wi_alloc(sizeof(*pn));
	if (!wi)
		return NULL;
	wi->wi_read		= net_read;
	wi->wi_write		= net_write;
	wi->wi_set_channel	= net_set_channel;
	wi->wi_get_channel	= net_get_channel;
        wi->wi_set_rate    	= net_set_rate;
	wi->wi_get_rate    	= net_get_rate;
	wi->wi_close		= net_close;
	wi->wi_fd		= net_fd;
	wi->wi_get_mac		= net_get_mac;
	wi->wi_get_monitor	= net_get_monitor;

	/* setup iface */
	s = do_net_open(iface);
	if (s == -1) {
		do_net_free(wi);
		return NULL;
	}

	/* setup private state */
	pn = wi_priv(wi);
	pn->pn_s = s;
	pn->pn_queue.q_next = pn->pn_queue.q_prev = &pn->pn_queue;
	pn->pn_queue_free.q_next = pn->pn_queue_free.q_prev
					= &pn->pn_queue_free;

	return wi;
}
