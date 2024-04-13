void qeth_clear_qdio_buffers(struct qeth_card *card)
{
	int i;

	QETH_CARD_TEXT(card, 2, "clearqdbf");
	/* clear outbound buffers to free skbs */
	for (i = 0; i < card->qdio.no_out_queues; ++i) {
		if (card->qdio.out_qs[i]) {
			qeth_clear_outq_buffers(card->qdio.out_qs[i], 0);
		}
	}
}
