smp_fetch_capture_req_uri(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct chunk *temp;
	struct http_txn *txn = smp->strm->txn;
	char *ptr;

	if (!txn || !txn->uri)
		return 0;

	ptr = txn->uri;

	while (*ptr != ' ' && *ptr != '\0')  /* find first space */
		ptr++;

	if (!*ptr)
		return 0;

	ptr++;  /* skip the space */

	temp = get_trash_chunk();
	ptr = temp->str = http_get_path_from_string(ptr);
	if (!ptr)
		return 0;
	while (*ptr != ' ' && *ptr != '\0')  /* find space after URI */
		ptr++;

	smp->data.u.str = *temp;
	smp->data.u.str.len = ptr - temp->str;
	smp->data.type = SMP_T_STR;
	smp->flags = SMP_F_CONST;

	return 1;
}
