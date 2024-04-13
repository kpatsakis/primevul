void qeth_close_dev(struct qeth_card *card)
{
	QETH_CARD_TEXT(card, 2, "cldevsubm");
	queue_work(qeth_wq, &card->close_dev_work);
}
