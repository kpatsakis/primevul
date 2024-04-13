static void qeth_configure_unitaddr(struct qeth_card *card, char *prcd)
{
	QETH_DBF_TEXT(SETUP, 2, "cfgunit");
	card->info.chpid = prcd[30];
	card->info.unit_addr2 = prcd[31];
	card->info.cula = prcd[63];
	card->info.guestlan = ((prcd[0x10] == _ascebc['V']) &&
			       (prcd[0x11] == _ascebc['M']));
}
