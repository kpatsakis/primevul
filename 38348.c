static inline int qeth_alloc_cq(struct qeth_card *card)
{
	int rc;

	if (card->options.cq == QETH_CQ_ENABLED) {
		int i;
		struct qdio_outbuf_state *outbuf_states;

		QETH_DBF_TEXT(SETUP, 2, "cqon");
		card->qdio.c_q = kzalloc(sizeof(struct qeth_qdio_q),
					 GFP_KERNEL);
		if (!card->qdio.c_q) {
			rc = -1;
			goto kmsg_out;
		}
		QETH_DBF_HEX(SETUP, 2, &card->qdio.c_q, sizeof(void *));

		for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; ++i) {
			card->qdio.c_q->bufs[i].buffer =
				&card->qdio.c_q->qdio_bufs[i];
		}

		card->qdio.no_in_queues = 2;

		card->qdio.out_bufstates =
			kzalloc(card->qdio.no_out_queues *
				QDIO_MAX_BUFFERS_PER_Q *
				sizeof(struct qdio_outbuf_state), GFP_KERNEL);
		outbuf_states = card->qdio.out_bufstates;
		if (outbuf_states == NULL) {
			rc = -1;
			goto free_cq_out;
		}
		for (i = 0; i < card->qdio.no_out_queues; ++i) {
			card->qdio.out_qs[i]->bufstates = outbuf_states;
			outbuf_states += QDIO_MAX_BUFFERS_PER_Q;
		}
	} else {
		QETH_DBF_TEXT(SETUP, 2, "nocq");
		card->qdio.c_q = NULL;
		card->qdio.no_in_queues = 1;
	}
	QETH_DBF_TEXT_(SETUP, 2, "iqc%d", card->qdio.no_in_queues);
	rc = 0;
out:
	return rc;
free_cq_out:
	kfree(card->qdio.c_q);
	card->qdio.c_q = NULL;
kmsg_out:
	dev_err(&card->gdev->dev, "Failed to create completion queue\n");
	goto out;
}
