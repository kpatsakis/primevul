static int qeth_qdio_establish(struct qeth_card *card)
{
	struct qdio_initialize init_data;
	char *qib_param_field;
	struct qdio_buffer **in_sbal_ptrs;
	void (**queue_start_poll) (struct ccw_device *, int, unsigned long);
	struct qdio_buffer **out_sbal_ptrs;
	int i, j, k;
	int rc = 0;

	QETH_DBF_TEXT(SETUP, 2, "qdioest");

	qib_param_field = kzalloc(QDIO_MAX_BUFFERS_PER_Q * sizeof(char),
			      GFP_KERNEL);
	if (!qib_param_field) {
		rc =  -ENOMEM;
		goto out_free_nothing;
	}

	qeth_create_qib_param_field(card, qib_param_field);
	qeth_create_qib_param_field_blkt(card, qib_param_field);

	in_sbal_ptrs = kzalloc(card->qdio.no_in_queues *
			       QDIO_MAX_BUFFERS_PER_Q * sizeof(void *),
			       GFP_KERNEL);
	if (!in_sbal_ptrs) {
		rc = -ENOMEM;
		goto out_free_qib_param;
	}
	for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; ++i) {
		in_sbal_ptrs[i] = (struct qdio_buffer *)
			virt_to_phys(card->qdio.in_q->bufs[i].buffer);
	}

	queue_start_poll = kzalloc(sizeof(void *) * card->qdio.no_in_queues,
				   GFP_KERNEL);
	if (!queue_start_poll) {
		rc = -ENOMEM;
		goto out_free_in_sbals;
	}
	for (i = 0; i < card->qdio.no_in_queues; ++i)
		queue_start_poll[i] = card->discipline->start_poll;

	qeth_qdio_establish_cq(card, in_sbal_ptrs, queue_start_poll);

	out_sbal_ptrs =
		kzalloc(card->qdio.no_out_queues * QDIO_MAX_BUFFERS_PER_Q *
			sizeof(void *), GFP_KERNEL);
	if (!out_sbal_ptrs) {
		rc = -ENOMEM;
		goto out_free_queue_start_poll;
	}
	for (i = 0, k = 0; i < card->qdio.no_out_queues; ++i)
		for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; ++j, ++k) {
			out_sbal_ptrs[k] = (struct qdio_buffer *)virt_to_phys(
				card->qdio.out_qs[i]->bufs[j]->buffer);
		}

	memset(&init_data, 0, sizeof(struct qdio_initialize));
	init_data.cdev                   = CARD_DDEV(card);
	init_data.q_format               = qeth_get_qdio_q_format(card);
	init_data.qib_param_field_format = 0;
	init_data.qib_param_field        = qib_param_field;
	init_data.no_input_qs            = card->qdio.no_in_queues;
	init_data.no_output_qs           = card->qdio.no_out_queues;
	init_data.input_handler 	 = card->discipline->input_handler;
	init_data.output_handler	 = card->discipline->output_handler;
	init_data.queue_start_poll_array = queue_start_poll;
	init_data.int_parm               = (unsigned long) card;
	init_data.input_sbal_addr_array  = (void **) in_sbal_ptrs;
	init_data.output_sbal_addr_array = (void **) out_sbal_ptrs;
	init_data.output_sbal_state_array = card->qdio.out_bufstates;
	init_data.scan_threshold =
		(card->info.type == QETH_CARD_TYPE_IQD) ? 1 : 32;

	if (atomic_cmpxchg(&card->qdio.state, QETH_QDIO_ALLOCATED,
		QETH_QDIO_ESTABLISHED) == QETH_QDIO_ALLOCATED) {
		rc = qdio_allocate(&init_data);
		if (rc) {
			atomic_set(&card->qdio.state, QETH_QDIO_ALLOCATED);
			goto out;
		}
		rc = qdio_establish(&init_data);
		if (rc) {
			atomic_set(&card->qdio.state, QETH_QDIO_ALLOCATED);
			qdio_free(CARD_DDEV(card));
		}
	}

	switch (card->options.cq) {
	case QETH_CQ_ENABLED:
		dev_info(&card->gdev->dev, "Completion Queue support enabled");
		break;
	case QETH_CQ_DISABLED:
		dev_info(&card->gdev->dev, "Completion Queue support disabled");
		break;
	default:
		break;
	}
out:
	kfree(out_sbal_ptrs);
out_free_queue_start_poll:
	kfree(queue_start_poll);
out_free_in_sbals:
	kfree(in_sbal_ptrs);
out_free_qib_param:
	kfree(qib_param_field);
out_free_nothing:
	return rc;
}
