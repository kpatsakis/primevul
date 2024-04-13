int qeth_send_startlan(struct qeth_card *card)
{
	int rc;
	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "strtlan");

	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_STARTLAN, 0);
	rc = qeth_send_ipa_cmd(card, iob, NULL, NULL);
	return rc;
}
