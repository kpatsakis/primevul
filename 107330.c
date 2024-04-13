compareChars(const widechar *address1, const widechar *address2, int count, int m,
		const TranslationTableHeader *table) {
	int k;
	if (!count) return 0;
	for (k = 0; k < count; k++)
		if ((findCharOrDots(address1[k], m, table))->lowercase !=
				(findCharOrDots(address2[k], m, table))->lowercase)
			return 0;
	return 1;
}
