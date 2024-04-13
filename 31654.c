smp_fetch_body_len(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_msg *msg;

	CHECK_HTTP_MESSAGE_FIRST();

	if ((smp->opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ)
		msg = &smp->strm->txn->req;
	else
		msg = &smp->strm->txn->rsp;

	smp->data.type = SMP_T_SINT;
	smp->data.u.sint = http_body_bytes(msg);

	smp->flags = SMP_F_VOL_TEST;
	return 1;
}
