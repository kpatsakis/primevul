smp_fetch_url_port(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                   const struct arg *args, struct sample *smp, const char *kw)
{
	struct http_txn *txn = l7;
	struct sockaddr_storage addr;

	CHECK_HTTP_MESSAGE_FIRST();

	url2sa(txn->req.chn->buf->p + txn->req.sl.rq.u, txn->req.sl.rq.u_l, &addr, NULL);
	if (((struct sockaddr_in *)&addr)->sin_family != AF_INET)
		return 0;

	smp->type = SMP_T_UINT;
	smp->data.uint = ntohs(((struct sockaddr_in *)&addr)->sin_port);
	smp->flags = 0;
	return 1;
}
