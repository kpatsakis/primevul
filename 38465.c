static void qeth_qdio_cq_handler(struct qeth_card *card,
		unsigned int qdio_err,
		unsigned int queue, int first_element, int count) {
	struct qeth_qdio_q *cq = card->qdio.c_q;
	int i;
	int rc;

	if (!qeth_is_cq(card, queue))
		goto out;

	QETH_CARD_TEXT_(card, 5, "qcqhe%d", first_element);
	QETH_CARD_TEXT_(card, 5, "qcqhc%d", count);
	QETH_CARD_TEXT_(card, 5, "qcqherr%d", qdio_err);

	if (qdio_err) {
		netif_stop_queue(card->dev);
		qeth_schedule_recovery(card);
		goto out;
	}

	if (card->options.performance_stats) {
		card->perf_stats.cq_cnt++;
		card->perf_stats.cq_start_time = qeth_get_micros();
	}

	for (i = first_element; i < first_element + count; ++i) {
		int bidx = i % QDIO_MAX_BUFFERS_PER_Q;
		struct qdio_buffer *buffer = &cq->qdio_bufs[bidx];
		int e;

		e = 0;
		while (buffer->element[e].addr) {
			unsigned long phys_aob_addr;

			phys_aob_addr = (unsigned long) buffer->element[e].addr;
			qeth_qdio_handle_aob(card, phys_aob_addr);
			buffer->element[e].addr = NULL;
			buffer->element[e].eflags = 0;
			buffer->element[e].sflags = 0;
			buffer->element[e].length = 0;

			++e;
		}

		buffer->element[15].eflags = 0;
		buffer->element[15].sflags = 0;
	}
	rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT, queue,
		    card->qdio.c_q->next_buf_to_init,
		    count);
	if (rc) {
		dev_warn(&card->gdev->dev,
			"QDIO reported an error, rc=%i\n", rc);
		QETH_CARD_TEXT(card, 2, "qcqherr");
	}
	card->qdio.c_q->next_buf_to_init = (card->qdio.c_q->next_buf_to_init
				   + count) % QDIO_MAX_BUFFERS_PER_Q;

	netif_wake_queue(card->dev);

	if (card->options.performance_stats) {
		int delta_t = qeth_get_micros();
		delta_t -= card->perf_stats.cq_start_time;
		card->perf_stats.cq_time += delta_t;
	}
out:
	return;
}
