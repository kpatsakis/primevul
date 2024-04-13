static void qeth_core_free_card(struct qeth_card *card)
{

	QETH_DBF_TEXT(SETUP, 2, "freecrd");
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));
	qeth_clean_channel(&card->read);
	qeth_clean_channel(&card->write);
	if (card->dev)
		free_netdev(card->dev);
	kfree(card->ip_tbd_list);
	qeth_free_qdio_buffers(card);
	unregister_service_level(&card->qeth_service_level);
	kfree(card);
}
