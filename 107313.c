static void queue_add(struct queue *head, struct queue *q)
{
	struct queue *pos = head->q_prev;

	q->q_prev = pos;
	q->q_next = pos->q_next;
	q->q_next->q_prev = q;
	pos->q_next = q;
}
