smp_fetch_capture_req_ver(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_txn *txn = smp->strm->txn;

	if (!txn || txn->req.msg_state < HTTP_MSG_HDR_FIRST)
		return 0;

	if (txn->req.flags & HTTP_MSGF_VER_11)
		smp->data.u.str.str = "HTTP/1.1";
	else
		smp->data.u.str.str = "HTTP/1.0";

	smp->data.u.str.len = 8;
	smp->data.type  = SMP_T_STR;
	smp->flags = SMP_F_CONST;
	return 1;

}
