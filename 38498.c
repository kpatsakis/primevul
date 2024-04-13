static int qeth_setadpparms_query_oat_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd;
	struct qeth_qoat_priv *priv;
	char *resdata;
	int resdatalen;

	QETH_CARD_TEXT(card, 3, "qoatcb");

	cmd = (struct qeth_ipa_cmd *)data;
	priv = (struct qeth_qoat_priv *)reply->param;
	resdatalen = cmd->data.setadapterparms.hdr.cmdlength;
	resdata = (char *)data + 28;

	if (resdatalen > (priv->buffer_len - priv->response_len)) {
		cmd->hdr.return_code = IPA_RC_FFFF;
		return 0;
	}

	memcpy((priv->buffer + priv->response_len), resdata,
		resdatalen);
	priv->response_len += resdatalen;

	if (cmd->data.setadapterparms.hdr.seq_no <
	    cmd->data.setadapterparms.hdr.used_total)
		return 1;
	return 0;
}
