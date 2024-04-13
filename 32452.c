set_next(peer_t *p, unsigned t)
{
	p->next_action_time = G.cur_time + t;
}
