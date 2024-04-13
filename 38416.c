static inline void qeth_free_cq(struct qeth_card *card)
{
	if (card->qdio.c_q) {
		--card->qdio.no_in_queues;
		kfree(card->qdio.c_q);
		card->qdio.c_q = NULL;
	}
	kfree(card->qdio.out_bufstates);
	card->qdio.out_bufstates = NULL;
}
