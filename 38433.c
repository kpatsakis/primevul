struct net_device_stats *qeth_get_stats(struct net_device *dev)
{
	struct qeth_card *card;

	card = dev->ml_priv;

	QETH_CARD_TEXT(card, 5, "getstat");

	return &card->stats;
}
