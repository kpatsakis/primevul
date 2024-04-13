static int qeth_core_thaw(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	if (card->discipline && card->discipline->thaw)
		return card->discipline->thaw(gdev);
	return 0;
}
