static inline int qeth_cq_init(struct qeth_card *card)
{
	int rc;

	if (card->options.cq == QETH_CQ_ENABLED) {
		QETH_DBF_TEXT(SETUP, 2, "cqinit");
		memset(card->qdio.c_q->qdio_bufs, 0,
		       QDIO_MAX_BUFFERS_PER_Q * sizeof(struct qdio_buffer));
		card->qdio.c_q->next_buf_to_init = 127;
		rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT,
			     card->qdio.no_in_queues - 1, 0,
			     127);
		if (rc) {
			QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
			goto out;
		}
	}
	rc = 0;
out:
	return rc;
}
