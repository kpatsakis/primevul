int qeth_configure_cq(struct qeth_card *card, enum qeth_cq cq)
{
	int rc;

	if (card->options.cq ==  QETH_CQ_NOTAVAILABLE) {
		rc = -1;
		goto out;
	} else {
		if (card->options.cq == cq) {
			rc = 0;
			goto out;
		}

		if (card->state != CARD_STATE_DOWN &&
		    card->state != CARD_STATE_RECOVER) {
			rc = -1;
			goto out;
		}

		qeth_free_qdio_buffers(card);
		card->options.cq = cq;
		rc = 0;
	}
out:
	return rc;

}
