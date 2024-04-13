static void http_server_error(struct stream *s, struct stream_interface *si,
			      int err, int finst, const struct chunk *msg)
{
	FLT_STRM_CB(s, flt_http_reply(s, s->txn->status, msg));
	channel_auto_read(si_oc(si));
	channel_abort(si_oc(si));
	channel_auto_close(si_oc(si));
	channel_erase(si_oc(si));
	channel_auto_close(si_ic(si));
	channel_auto_read(si_ic(si));
	if (msg)
		co_inject(si_ic(si), msg->str, msg->len);
	if (!(s->flags & SF_ERR_MASK))
		s->flags |= err;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= finst;
}
