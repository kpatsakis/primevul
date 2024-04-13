static int qeth_determine_card_type(struct qeth_card *card)
{
	int i = 0;

	QETH_DBF_TEXT(SETUP, 2, "detcdtyp");

	card->qdio.do_prio_queueing = QETH_PRIOQ_DEFAULT;
	card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	while (known_devices[i][QETH_DEV_MODEL_IND]) {
		if ((CARD_RDEV(card)->id.dev_type ==
				known_devices[i][QETH_DEV_TYPE_IND]) &&
		    (CARD_RDEV(card)->id.dev_model ==
				known_devices[i][QETH_DEV_MODEL_IND])) {
			card->info.type = known_devices[i][QETH_DEV_MODEL_IND];
			card->qdio.no_out_queues =
				known_devices[i][QETH_QUEUE_NO_IND];
			card->qdio.no_in_queues = 1;
			card->info.is_multicast_different =
				known_devices[i][QETH_MULTICAST_IND];
			qeth_update_from_chp_desc(card);
			return 0;
		}
		i++;
	}
	card->info.type = QETH_CARD_TYPE_UNKNOWN;
	dev_err(&card->gdev->dev, "The adapter hardware is of an "
		"unknown type\n");
	return -ENOENT;
}
