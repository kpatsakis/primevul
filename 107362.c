onlyLettersAhead(const TranslationTableHeader *table, int pos, const InString *input,
		int transCharslen, TranslationTableCharacterAttributes afterAttributes) {
	/* Actullly, spaces, then letters */
	int k;
	if (!(afterAttributes & CTC_Space)) return 0;
	for (k = pos + transCharslen + 1; k < input->length; k++) {
		TranslationTableCharacterAttributes attr =
				(findCharOrDots(input->chars[k], 0, table))->attributes;
		if ((attr & CTC_Space)) continue;
		if ((attr & (CTC_Letter | CTC_LitDigit)))
			return 1;
		else
			return 0;
	}
	return 0;
}
