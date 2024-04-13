lou_hyphenate(const char *tableList, const widechar *inbuf, int inlen, char *hyphens,
		int mode) {
#define HYPHSTRING 100
	const TranslationTableHeader *table;
	widechar workingBuffer[HYPHSTRING];
	int k, kk;
	int wordStart;
	int wordEnd;
	table = lou_getTable(tableList);
	if (table == NULL || inbuf == NULL || hyphens == NULL ||
			table->hyphenStatesArray == 0 || inlen >= HYPHSTRING)
		return 0;
	if (mode != 0) {
		k = inlen;
		kk = HYPHSTRING;
		if (!lou_backTranslate(tableList, inbuf, &k, &workingBuffer[0], &kk, NULL, NULL,
					NULL, NULL, NULL, 0))
			return 0;
	} else {
		memcpy(&workingBuffer[0], inbuf, CHARSIZE * inlen);
		kk = inlen;
	}
	for (wordStart = 0; wordStart < kk; wordStart++)
		if (((findCharOrDots(workingBuffer[wordStart], 0, table))->attributes &
					CTC_Letter))
			break;
	if (wordStart == kk) return 0;
	for (wordEnd = kk - 1; wordEnd >= 0; wordEnd--)
		if (((findCharOrDots(workingBuffer[wordEnd], 0, table))->attributes & CTC_Letter))
			break;
	for (k = wordStart; k <= wordEnd; k++) {
		TranslationTableCharacter *c = findCharOrDots(workingBuffer[k], 0, table);
		if (!(c->attributes & CTC_Letter)) return 0;
	}
	if (!hyphenate(&workingBuffer[wordStart], wordEnd - wordStart + 1,
				&hyphens[wordStart], table))
		return 0;
	for (k = 0; k <= wordStart; k++) hyphens[k] = '0';
	if (mode != 0) {
		widechar workingBuffer2[HYPHSTRING];
		int outputPos[HYPHSTRING];
		char hyphens2[HYPHSTRING];
		kk = wordEnd - wordStart + 1;
		k = HYPHSTRING;
		if (!lou_translate(tableList, &workingBuffer[wordStart], &kk, &workingBuffer2[0],
					&k, NULL, NULL, &outputPos[0], NULL, NULL, 0))
			return 0;
		for (kk = 0; kk < k; kk++) {
			int hyphPos = outputPos[kk];
			if (hyphPos > k || hyphPos < 0) break;
			if (hyphens[wordStart + kk] & 1)
				hyphens2[hyphPos] = '1';
			else
				hyphens2[hyphPos] = '0';
		}
		for (kk = wordStart; kk < wordStart + k; kk++)
			if (hyphens2[kk] == '0') hyphens[kk] = hyphens2[kk];
	}
	for (k = 0; k < inlen; k++)
		if (hyphens[k] & 1)
			hyphens[k] = '1';
		else
			hyphens[k] = '0';
	hyphens[inlen] = 0;
	return 1;
}
