void http_end_txn(struct stream *s)
{
	struct http_txn *txn = s->txn;
	struct proxy *fe = strm_fe(s);

	/* these ones will have been dynamically allocated */
	pool_free(pool_head_requri, txn->uri);
	pool_free(pool_head_capture, txn->cli_cookie);
	pool_free(pool_head_capture, txn->srv_cookie);
	pool_free(pool_head_uniqueid, s->unique_id);

	s->unique_id = NULL;
	txn->uri = NULL;
	txn->srv_cookie = NULL;
	txn->cli_cookie = NULL;

	if (s->req_cap) {
		struct cap_hdr *h;
		for (h = fe->req_cap; h; h = h->next)
			pool_free(h->pool, s->req_cap[h->index]);
		memset(s->req_cap, 0, fe->nb_req_cap * sizeof(void *));
	}

	if (s->res_cap) {
		struct cap_hdr *h;
		for (h = fe->rsp_cap; h; h = h->next)
			pool_free(h->pool, s->res_cap[h->index]);
		memset(s->res_cap, 0, fe->nb_rsp_cap * sizeof(void *));
	}

	vars_prune(&s->vars_txn, s->sess, s);
	vars_prune(&s->vars_reqres, s->sess, s);
}
