enum act_return http_action_reject(struct act_rule *rule, struct proxy *px,
                                   struct session *sess, struct stream *s, int flags)
{
	channel_abort(&s->req);
	channel_abort(&s->res);
	s->req.analysers = 0;
	s->res.analysers = 0;

	HA_ATOMIC_ADD(&s->be->be_counters.denied_req, 1);
	HA_ATOMIC_ADD(&sess->fe->fe_counters.denied_req, 1);
	if (sess->listener && sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->denied_req, 1);

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_PRXCOND;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= SF_FINST_R;

	return ACT_RET_CONT;
}
