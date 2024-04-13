static int qeth_core_freeze(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	if (card->discipline && card->discipline->freeze)
		return card->discipline->freeze(gdev);
	return 0;
}
