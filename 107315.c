static int queue_get(struct priv_net *pn, void *buf, int len)
{
	struct queue *head = &pn->pn_queue;
	struct queue *q = head->q_next;

	if (q == head)
		return 0;

	assert(q->q_len <= len);
	memcpy(buf, q->q_buf, q->q_len);

	queue_del(q);
	queue_add(&pn->pn_queue_free, q);

	return q->q_len;
}
