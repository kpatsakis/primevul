smp_fetch_capture_req_uri(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                             const struct arg *args, struct sample *smp, const char *kw)
{
	struct chunk *temp;
	struct http_txn *txn = l7;
	char *ptr;

	if (!txn->uri)
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

	smp->data.str = *temp;
	smp->data.str.len = ptr - temp->str;
	smp->type = SMP_T_STR;
	smp->flags = SMP_F_CONST;

	return 1;
}
