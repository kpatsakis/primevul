static void queue_del(struct queue *q)
{
	q->q_prev->q_next = q->q_next;
	q->q_next->q_prev = q->q_prev;
}
