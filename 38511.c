static void qeth_update_from_chp_desc(struct qeth_card *card)
{
	struct ccw_device *ccwdev;
	struct channelPath_dsc {
		u8 flags;
		u8 lsn;
		u8 desc;
		u8 chpid;
		u8 swla;
		u8 zeroes;
		u8 chla;
		u8 chpp;
	} *chp_dsc;

	QETH_DBF_TEXT(SETUP, 2, "chp_desc");

	ccwdev = card->data.ccwdev;
	chp_dsc = ccw_device_get_chp_desc(ccwdev, 0);
	if (!chp_dsc)
		goto out;

	card->info.func_level = 0x4100 + chp_dsc->desc;
	if (card->info.type == QETH_CARD_TYPE_IQD)
		goto out;

	/* CHPP field bit 6 == 1 -> single queue */
	if ((chp_dsc->chpp & 0x02) == 0x02)
		qeth_set_single_write_queues(card);
	else
		qeth_set_multiple_write_queues(card);
out:
	kfree(chp_dsc);
	QETH_DBF_TEXT_(SETUP, 2, "nr:%x", card->qdio.no_out_queues);
	QETH_DBF_TEXT_(SETUP, 2, "lvl:%02x", card->info.func_level);
}
