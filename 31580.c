struct http_txn *http_alloc_txn(struct stream *s)
{
	struct http_txn *txn = s->txn;

	if (txn)
		return txn;

	txn = pool_alloc(pool_head_http_txn);
	if (!txn)
		return txn;

	txn->hdr_idx.size = global.tune.max_http_hdr;
	txn->hdr_idx.v    = pool_alloc(pool_head_hdr_idx);
	if (!txn->hdr_idx.v) {
		pool_free(pool_head_http_txn, txn);
		return NULL;
	}

	s->txn = txn;
	return txn;
}
