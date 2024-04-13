static struct queue *queue_get_slot(struct priv_net *pn)
{
	struct queue *q = pn->pn_queue_free.q_next;

	if (q != &pn->pn_queue_free) {
		queue_del(q);
		return q;
	}

	if (pn->pn_queue_len++ > QUEUE_MAX)
		return NULL;

	return malloc(sizeof(*q));
}
