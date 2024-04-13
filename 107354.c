lou_dotsToChar(
		const char *tableList, widechar *inbuf, widechar *outbuf, int length, int mode) {
	const TranslationTableHeader *table;
	int k;
	widechar dots;
	if (tableList == NULL || inbuf == NULL || outbuf == NULL) return 0;

	table = lou_getTable(tableList);
	if (table == NULL || length <= 0) return 0;
	for (k = 0; k < length; k++) {
		dots = inbuf[k];
		if (!(dots & B16) && (dots & 0xff00) == 0x2800) /* Unicode braille */
			dots = (dots & 0x00ff) | B16;
		outbuf[k] = _lou_getCharFromDots(dots);
	}
	return 1;
}
