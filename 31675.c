smp_fetch_http_first_req(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	smp->data.type = SMP_T_BOOL;
	smp->data.u.sint = !(smp->strm->txn->flags & TX_NOT_FIRST);
	return 1;
}
