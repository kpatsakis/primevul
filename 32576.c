smp_fetch_capture_header_res(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                 const struct arg *args, struct sample *smp, const char *kw)
{
	struct proxy *fe = l4->fe;
	struct http_txn *txn = l7;
	int idx;

	if (!args || args->type != ARGT_UINT)
		return 0;

	idx = args->data.uint;

	if (idx > (fe->nb_rsp_cap - 1) || txn->rsp.cap == NULL || txn->rsp.cap[idx] == NULL)
		return 0;

	smp->type = SMP_T_STR;
	smp->flags |= SMP_F_CONST;
	smp->data.str.str = txn->rsp.cap[idx];
	smp->data.str.len = strlen(txn->rsp.cap[idx]);

	return 1;
}
