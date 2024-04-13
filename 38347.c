static struct qeth_card *qeth_alloc_card(void)
{
	struct qeth_card *card;

	QETH_DBF_TEXT(SETUP, 2, "alloccrd");
	card = kzalloc(sizeof(struct qeth_card), GFP_DMA|GFP_KERNEL);
	if (!card)
		goto out;
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));
	card->ip_tbd_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
	if (!card->ip_tbd_list) {
		QETH_DBF_TEXT(SETUP, 0, "iptbdnom");
		goto out_card;
	}
	if (qeth_setup_channel(&card->read))
		goto out_ip;
	if (qeth_setup_channel(&card->write))
		goto out_channel;
	card->options.layer2 = -1;
	card->qeth_service_level.seq_print = qeth_core_sl_print;
	register_service_level(&card->qeth_service_level);
	return card;

out_channel:
	qeth_clean_channel(&card->read);
out_ip:
	kfree(card->ip_tbd_list);
out_card:
	kfree(card);
out:
	return NULL;
}
