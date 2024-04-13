static void qeth_set_multiple_write_queues(struct qeth_card *card)
{
	if ((atomic_read(&card->qdio.state) != QETH_QDIO_UNINITIALIZED) &&
	    (card->qdio.no_out_queues == 1)) {
		qeth_free_qdio_buffers(card);
		card->qdio.default_out_queue = 2;
	}
	card->qdio.no_out_queues = 4;
}
