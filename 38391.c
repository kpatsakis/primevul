static int qeth_core_probe_device(struct ccwgroup_device *gdev)
{
	struct qeth_card *card;
	struct device *dev;
	int rc;
	unsigned long flags;
	char dbf_name[DBF_NAME_LEN];

	QETH_DBF_TEXT(SETUP, 2, "probedev");

	dev = &gdev->dev;
	if (!get_device(dev))
		return -ENODEV;

	QETH_DBF_TEXT_(SETUP, 2, "%s", dev_name(&gdev->dev));

	card = qeth_alloc_card();
	if (!card) {
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", -ENOMEM);
		rc = -ENOMEM;
		goto err_dev;
	}

	snprintf(dbf_name, sizeof(dbf_name), "qeth_card_%s",
		dev_name(&gdev->dev));
	card->debug = qeth_get_dbf_entry(dbf_name);
	if (!card->debug) {
		rc = qeth_add_dbf_entry(card, dbf_name);
		if (rc)
			goto err_card;
	}

	card->read.ccwdev  = gdev->cdev[0];
	card->write.ccwdev = gdev->cdev[1];
	card->data.ccwdev  = gdev->cdev[2];
	dev_set_drvdata(&gdev->dev, card);
	card->gdev = gdev;
	gdev->cdev[0]->handler = qeth_irq;
	gdev->cdev[1]->handler = qeth_irq;
	gdev->cdev[2]->handler = qeth_irq;

	rc = qeth_determine_card_type(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "3err%d", rc);
		goto err_card;
	}
	rc = qeth_setup_card(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "2err%d", rc);
		goto err_card;
	}

	if (card->info.type == QETH_CARD_TYPE_OSN)
		gdev->dev.type = &qeth_osn_devtype;
	else
		gdev->dev.type = &qeth_generic_devtype;

	switch (card->info.type) {
	case QETH_CARD_TYPE_OSN:
	case QETH_CARD_TYPE_OSM:
		rc = qeth_core_load_discipline(card, QETH_DISCIPLINE_LAYER2);
		if (rc)
			goto err_card;
		rc = card->discipline->setup(card->gdev);
		if (rc)
			goto err_disc;
	case QETH_CARD_TYPE_OSD:
	case QETH_CARD_TYPE_OSX:
	default:
		break;
	}

	write_lock_irqsave(&qeth_core_card_list.rwlock, flags);
	list_add_tail(&card->list, &qeth_core_card_list.list);
	write_unlock_irqrestore(&qeth_core_card_list.rwlock, flags);

	qeth_determine_capabilities(card);
	return 0;

err_disc:
	qeth_core_free_discipline(card);
err_card:
	qeth_core_free_card(card);
err_dev:
	put_device(dev);
	return rc;
}
