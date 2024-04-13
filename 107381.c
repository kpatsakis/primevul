syllableBreak(const TranslationTableHeader *table, int pos, const InString *input,
		int transCharslen) {
	int wordStart = 0;
	int wordEnd = 0;
	int wordSize = 0;
	int k = 0;
	char *hyphens = NULL;
	for (wordStart = pos; wordStart >= 0; wordStart--)
		if (!((findCharOrDots(input->chars[wordStart], 0, table))->attributes &
					CTC_Letter)) {
			wordStart++;
			break;
		}
	if (wordStart < 0) wordStart = 0;
	for (wordEnd = pos; wordEnd < input->length; wordEnd++)
		if (!((findCharOrDots(input->chars[wordEnd], 0, table))->attributes &
					CTC_Letter)) {
			wordEnd--;
			break;
		}
	if (wordEnd == input->length) wordEnd--;
	/* At this stage wordStart is the 0 based index of the first letter in the word,
	 * wordEnd is the 0 based index of the last letter in the word.
	 * example: "hello" wordstart=0, wordEnd=4. */
	wordSize = wordEnd - wordStart + 1;
	hyphens = (char *)calloc(wordSize + 1, sizeof(char));
	if (!hyphenate(&input->chars[wordStart], wordSize, hyphens, table)) {
		free(hyphens);
		return 0;
	}
	for (k = pos - wordStart + 1; k < (pos - wordStart + transCharslen); k++)
		if (hyphens[k] & 1) {
			free(hyphens);
			return 1;
		}
	free(hyphens);
	return 0;
}
