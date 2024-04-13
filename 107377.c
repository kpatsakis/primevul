setAfter(int length, const TranslationTableHeader *table, int pos, const InString *input,
		TranslationTableCharacterAttributes *afterAttributes) {
	widechar after;
	if ((pos + length + 2) < input->length && input->chars[pos + 1] == ENDSEGMENT)
		after = input->chars[pos + 2];
	else
		after = (pos + length < input->length) ? input->chars[pos + length] : ' ';
	*afterAttributes = (findCharOrDots(after, 0, table))->attributes;
}
