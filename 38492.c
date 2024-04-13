static void qeth_set_single_write_queues(struct qeth_card *card)
{
	if ((atomic_read(&card->qdio.state) != QETH_QDIO_UNINITIALIZED) &&
	    (card->qdio.no_out_queues == 4))
		qeth_free_qdio_buffers(card);

	card->qdio.no_out_queues = 1;
	if (card->qdio.default_out_queue != 0)
		dev_info(&card->gdev->dev, "Priority Queueing not supported\n");

	card->qdio.default_out_queue = 0;
}
