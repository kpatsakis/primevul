static int qeth_qdio_activate(struct qeth_card *card)
{
	QETH_DBF_TEXT(SETUP, 3, "qdioact");
	return qdio_activate(CARD_DDEV(card));
}
