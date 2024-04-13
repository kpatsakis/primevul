smp_fetch_capture_req_method(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct chunk *temp;
	struct http_txn *txn = smp->strm->txn;
	char *ptr;

	if (!txn || !txn->uri)
		return 0;

	ptr = txn->uri;

	while (*ptr != ' ' && *ptr != '\0')  /* find first space */
		ptr++;

	temp = get_trash_chunk();
	temp->str = txn->uri;
	temp->len = ptr - txn->uri;
	smp->data.u.str = *temp;
	smp->data.type = SMP_T_STR;
	smp->flags = SMP_F_CONST;

	return 1;

}
