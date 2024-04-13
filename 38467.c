static inline void qeth_qdio_establish_cq(struct qeth_card *card,
	struct qdio_buffer **in_sbal_ptrs,
	void (**queue_start_poll) (struct ccw_device *, int, unsigned long)) {
	int i;

	if (card->options.cq == QETH_CQ_ENABLED) {
		int offset = QDIO_MAX_BUFFERS_PER_Q *
			     (card->qdio.no_in_queues - 1);
		i = QDIO_MAX_BUFFERS_PER_Q * (card->qdio.no_in_queues - 1);
		for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; ++i) {
			in_sbal_ptrs[offset + i] = (struct qdio_buffer *)
				virt_to_phys(card->qdio.c_q->bufs[i].buffer);
		}

		queue_start_poll[card->qdio.no_in_queues - 1] = NULL;
	}
}
