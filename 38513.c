int qeth_wait_for_threads(struct qeth_card *card, unsigned long threads)
{
	if (qeth_is_recovery_task(card))
		return 0;
	return wait_event_interruptible(card->wait_q,
			qeth_threads_running(card, threads) == 0);
}
