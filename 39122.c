static int check_restart(struct sem_array *sma, struct sem_queue *q)
{
	struct sem *curr;
	struct sem_queue *h;

	/* if the operation didn't modify the array, then no restart */
	if (q->alter == 0)
		return 0;

	/* pending complex operations are too difficult to analyse */
	if (sma->complex_count)
		return 1;

	/* we were a sleeping complex operation. Too difficult */
	if (q->nsops > 1)
		return 1;

	curr = sma->sem_base + q->sops[0].sem_num;

	/* No-one waits on this queue */
	if (list_empty(&curr->sem_pending))
		return 0;

	/* the new semaphore value */
	if (curr->semval) {
		/* It is impossible that someone waits for the new value:
		 * - q is a previously sleeping simple operation that
		 *   altered the array. It must be a decrement, because
		 *   simple increments never sleep.
		 * - The value is not 0, thus wait-for-zero won't proceed.
		 * - If there are older (higher priority) decrements
		 *   in the queue, then they have observed the original
		 *   semval value and couldn't proceed. The operation
		 *   decremented to value - thus they won't proceed either.
		 */
		BUG_ON(q->sops[0].sem_op >= 0);
		return 0;
	}
	/*
	 * semval is 0. Check if there are wait-for-zero semops.
	 * They must be the first entries in the per-semaphore queue
	 */
	h = list_first_entry(&curr->sem_pending, struct sem_queue, list);
	BUG_ON(h->nsops != 1);
	BUG_ON(h->sops[0].sem_num != q->sops[0].sem_num);

	/* Yes, there is a wait-for-zero semop. Restart */
	if (h->sops[0].sem_op == 0)
		return 1;

	/* Again - no-one is waiting for the new value. */
	return 0;
}
