void http_reset_txn(struct stream *s)
{
	http_end_txn(s);
	http_init_txn(s);

	/* reinitialise the current rule list pointer to NULL. We are sure that
	 * any rulelist match the NULL pointer.
	 */
	s->current_rule_list = NULL;

	s->be = strm_fe(s);
	s->logs.logwait = strm_fe(s)->to_log;
	s->logs.level = 0;
	stream_del_srv_conn(s);
	s->target = NULL;
	/* re-init store persistence */
	s->store_count = 0;
	s->uniq_id = global.req_count++;

	s->req.flags |= CF_READ_DONTWAIT; /* one read is usually enough */

	/* We must trim any excess data from the response buffer, because we
	 * may have blocked an invalid response from a server that we don't
	 * want to accidentely forward once we disable the analysers, nor do
	 * we want those data to come along with next response. A typical
	 * example of such data would be from a buggy server responding to
	 * a HEAD with some data, or sending more than the advertised
	 * content-length.
	 */
	if (unlikely(s->res.buf->i))
		s->res.buf->i = 0;

	/* Now we can realign the response buffer */
	buffer_realign(s->res.buf);

	s->req.rto = strm_fe(s)->timeout.client;
	s->req.wto = TICK_ETERNITY;

	s->res.rto = TICK_ETERNITY;
	s->res.wto = strm_fe(s)->timeout.client;

	s->req.rex = TICK_ETERNITY;
	s->req.wex = TICK_ETERNITY;
	s->req.analyse_exp = TICK_ETERNITY;
	s->res.rex = TICK_ETERNITY;
	s->res.wex = TICK_ETERNITY;
	s->res.analyse_exp = TICK_ETERNITY;
	s->si[1].hcto = TICK_ETERNITY;
}
