isRepeatedWord(const TranslationTableHeader *table, int pos, const InString *input,
		int transCharslen, const widechar **repwordStart, int *repwordLength) {
	int start;
	if (pos == 0 || !checkAttr(input->chars[pos - 1], CTC_Letter, 0, table)) return 0;
	if ((pos + transCharslen) >= input->length ||
			!checkAttr(input->chars[pos + transCharslen], CTC_Letter, 0, table))
		return 0;
	for (start = pos - 2;
			start >= 0 && checkAttr(input->chars[start], CTC_Letter, 0, table); start--)
		;
	start++;
	*repwordStart = &input->chars[start];
	*repwordLength = pos - start;
	if (compareChars(*repwordStart, &input->chars[pos + transCharslen], *repwordLength, 0,
				table))
		return 1;
	return 0;
}
