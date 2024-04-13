lou_charToDots(const char *tableList, const widechar *inbuf, widechar *outbuf, int length,
		int mode) {
	const TranslationTableHeader *table;
	int k;
	if (tableList == NULL || inbuf == NULL || outbuf == NULL) return 0;

	table = lou_getTable(tableList);
	if (table == NULL || length <= 0) return 0;
	for (k = 0; k < length; k++)
		if ((mode & ucBrl))
			outbuf[k] = ((_lou_getDotsForChar(inbuf[k]) & 0xff) | 0x2800);
		else
			outbuf[k] = _lou_getDotsForChar(inbuf[k]);
	return 1;
}
