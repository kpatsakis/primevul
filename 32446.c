resolve_peer_hostname(peer_t *p, int loop_on_fail)
{
	len_and_sockaddr *lsa;

 again:
	lsa = host2sockaddr(p->p_hostname, 123);
	if (!lsa) {
		/* error message already emitted by host2sockaddr() */
		if (!loop_on_fail)
			return;
		sleep(5);
		goto again;
	}
	free(p->p_lsa);
	free(p->p_dotted);
	p->p_lsa = lsa;
	p->p_dotted = xmalloc_sockaddr2dotted_noport(&lsa->u.sa);
}
