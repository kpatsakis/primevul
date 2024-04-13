copyCharacters(int from, int to, const TranslationTableHeader *table,
		const InString *input, OutString *output, int *posMapping, int transOpcode,
		int *cursorPosition, int *cursorStatus) {
	if (transOpcode == CTO_Context) {
		while (from < to) {
			if (!putCharacter(input->chars[from], table, from, input, output, posMapping,
						cursorPosition, cursorStatus))
				return 0;
			from++;
		}
	} else {
		if (to > from) {
			if ((output->length + to - from) > output->maxlength) return 0;
			while (to > from) {
				posMapping[output->length] = from;
				output->chars[output->length] = input->chars[from];
				output->length++;
				from++;
			}
		}
	}

	return 1;
}
