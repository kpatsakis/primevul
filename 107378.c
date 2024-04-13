setBefore(const TranslationTableHeader *table, int pos, const InString *input,
		TranslationTableCharacterAttributes *beforeAttributes) {
	widechar before;
	if (pos >= 2 && input->chars[pos - 1] == ENDSEGMENT)
		before = input->chars[pos - 2];
	else
		before = (pos == 0) ? ' ' : input->chars[pos - 1];
	*beforeAttributes = (findCharOrDots(before, 0, table))->attributes;
}
