static void http_server_error(struct session *s, struct stream_interface *si,
			      int err, int finst, int status, const struct chunk *msg)
{
	channel_auto_read(si->ob);
	channel_abort(si->ob);
	channel_auto_close(si->ob);
	channel_erase(si->ob);
	channel_auto_close(si->ib);
	channel_auto_read(si->ib);
	if (status > 0 && msg) {
		s->txn.status = status;
		bo_inject(si->ib, msg->str, msg->len);
	}
	if (!(s->flags & SN_ERR_MASK))
		s->flags |= err;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= finst;
}
