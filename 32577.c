smp_fetch_capture_req_method(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
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

	temp = get_trash_chunk();
	temp->str = txn->uri;
	temp->len = ptr - txn->uri;
	smp->data.str = *temp;
	smp->type = SMP_T_STR;
	smp->flags = SMP_F_CONST;

	return 1;

}
