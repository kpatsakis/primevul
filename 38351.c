void qeth_buffer_reclaim_work(struct work_struct *work)
{
	struct qeth_card *card = container_of(work, struct qeth_card,
		buffer_reclaim_work.work);

	QETH_CARD_TEXT_(card, 2, "brw:%x", card->reclaim_index);
	qeth_queue_input_buffer(card, card->reclaim_index);
}
