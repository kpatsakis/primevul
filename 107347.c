insertEmphases(const TranslationTableHeader *table, int pos, const InString *input,
		OutString *output, int *posMapping, const EmphasisInfo *emphasisBuffer,
		int haveEmphasis, int transOpcode, int *cursorPosition, int *cursorStatus,
		int *pre_src) {
	int at;

	for (at = *pre_src; at <= pos; at++)
		insertEmphasesAt(at, table, pos, input, output, posMapping, emphasisBuffer,
				haveEmphasis, transOpcode, cursorPosition, cursorStatus);

	*pre_src = pos + 1;
}
