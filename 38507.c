void qeth_trace_features(struct qeth_card *card)
{
	QETH_CARD_TEXT(card, 2, "features");
	QETH_CARD_TEXT_(card, 2, "%x", card->options.ipa4.supported_funcs);
	QETH_CARD_TEXT_(card, 2, "%x", card->options.ipa4.enabled_funcs);
	QETH_CARD_TEXT_(card, 2, "%x", card->options.ipa6.supported_funcs);
	QETH_CARD_TEXT_(card, 2, "%x", card->options.ipa6.enabled_funcs);
	QETH_CARD_TEXT_(card, 2, "%x", card->options.adp.supported_funcs);
	QETH_CARD_TEXT_(card, 2, "%x", card->options.adp.enabled_funcs);
	QETH_CARD_TEXT_(card, 2, "%x", card->info.diagass_support);
}
