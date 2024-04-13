void qeth_clear_recovery_task(struct qeth_card *card)
{
	card->recovery_task = NULL;
}
