static void qeth_close_dev_handler(struct work_struct *work)
{
	struct qeth_card *card;

	card = container_of(work, struct qeth_card, close_dev_work);
	QETH_CARD_TEXT(card, 2, "cldevhdl");
	rtnl_lock();
	dev_close(card->dev);
	rtnl_unlock();
	ccwgroup_set_offline(card->gdev);
}
