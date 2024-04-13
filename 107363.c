onlyLettersBehind(const TranslationTableHeader *table, int pos, const InString *input,
		TranslationTableCharacterAttributes beforeAttributes) {
	/* Actually, spaces, then letters */
	int k;
	if (!(beforeAttributes & CTC_Space)) return 0;
	for (k = pos - 2; k >= 0; k--) {
		TranslationTableCharacterAttributes attr =
				(findCharOrDots(input->chars[k], 0, table))->attributes;
		if ((attr & CTC_Space)) continue;
		if ((attr & CTC_Letter))
			return 1;
		else
			return 0;
	}
	return 1;
}
