doCompbrl(const TranslationTableHeader *table, int *pos, const InString *input,
		OutString *output, int *posMapping, EmphasisInfo *emphasisBuffer,
		const TranslationTableRule **transRule, int *cursorPosition, int *cursorStatus,
		int destword, int srcword) {
	/* Handle strings containing substrings defined by the compbrl opcode */
	int stringStart, stringEnd;
	if (checkAttr(input->chars[*pos], CTC_Space, 0, table)) return 1;
	if (destword) {
		*pos = srcword;
		output->length = destword;
	} else {
		*pos = 0;
		output->length = 0;
	}
	for (stringStart = *pos; stringStart >= 0; stringStart--)
		if (checkAttr(input->chars[stringStart], CTC_Space, 0, table)) break;
	stringStart++;
	for (stringEnd = *pos; stringEnd < input->length; stringEnd++)
		if (checkAttr(input->chars[stringEnd], CTC_Space, 0, table)) break;
	return doCompTrans(stringStart, stringEnd, table, pos, input, output, posMapping,
			emphasisBuffer, transRule, cursorPosition, cursorStatus);
}
