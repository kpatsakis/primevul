insertEmphasesAt(const int at, const TranslationTableHeader *table, int pos,
		const InString *input, OutString *output, int *posMapping,
		const EmphasisInfo *emphasisBuffer, int haveEmphasis, int transOpcode,
		int *cursorPosition, int *cursorStatus) {
	int type_counts[10];
	int i, j, min, max;

	/* simple case */
	if (!haveEmphasis) {
		/* insert graded 1 mode indicator */
		if (transOpcode == CTO_Contraction) {
			const TranslationTableRule *indicRule;
			if (brailleIndicatorDefined(table->noContractSign, table, &indicRule))
				for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0,
						pos, input, output, posMapping, cursorPosition, cursorStatus);
		}

		if ((emphasisBuffer[at].begin | emphasisBuffer[at].end | emphasisBuffer[at].word |
					emphasisBuffer[at].symbol) &
				capsEmphClass) {
			insertEmphasisEnd(emphasisBuffer, at, capsRule, capsEmphClass, table, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
			insertEmphasisBegin(emphasisBuffer, at, capsRule, capsEmphClass, table, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
			insertEmphasisSymbol(emphasisBuffer, at, capsRule, capsEmphClass, table, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
		}
		return;
	}

	/* The order of inserting the end symbols must be the reverse
	 * of the insertions of the begin symbols so that they will
	 * nest properly when multiple emphases start and end at
	 * the same place */

	if ((emphasisBuffer[at].begin | emphasisBuffer[at].end | emphasisBuffer[at].word |
				emphasisBuffer[at].symbol) &
			capsEmphClass)
		insertEmphasisEnd(emphasisBuffer, at, capsRule, capsEmphClass, table, pos, input,
				output, posMapping, cursorPosition, cursorStatus);

	/* end bits */
	for (i = 0; i < 10; i++)
		type_counts[i] = endCount(emphasisBuffer, at, emphClasses[i]);

	for (i = 0; i < 10; i++) {
		min = -1;
		for (j = 0; j < 10; j++)
			if (type_counts[j] > 0)
				if (min < 0 || type_counts[j] < type_counts[min]) min = j;
		if (min < 0) break;
		type_counts[min] = 0;
		insertEmphasisEnd(emphasisBuffer, at, emph1Rule + min, emphClasses[min], table,
				pos, input, output, posMapping, cursorPosition, cursorStatus);
	}

	/* begin and word bits */
	for (i = 0; i < 10; i++)
		type_counts[i] = beginCount(emphasisBuffer, at, emphClasses[i], table, input);

	for (i = 9; i >= 0; i--) {
		max = 9;
		for (j = 9; j >= 0; j--)
			if (type_counts[max] < type_counts[j]) max = j;
		if (!type_counts[max]) break;
		type_counts[max] = 0;
		insertEmphasisBegin(emphasisBuffer, at, emph1Rule + max, emphClasses[max], table,
				pos, input, output, posMapping, cursorPosition, cursorStatus);
	}

	/* symbol bits */
	for (i = 9; i >= 0; i--)
		if ((emphasisBuffer[at].begin | emphasisBuffer[at].end | emphasisBuffer[at].word |
					emphasisBuffer[at].symbol) &
				emphClasses[i])
			insertEmphasisSymbol(emphasisBuffer, at, emph1Rule + i, emphClasses[i], table,
					pos, input, output, posMapping, cursorPosition, cursorStatus);

	/* insert graded 1 mode indicator */
	if (transOpcode == CTO_Contraction) {
		const TranslationTableRule *indicRule;
		if (brailleIndicatorDefined(table->noContractSign, table, &indicRule))
			for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
	}

	/* insert capitalization last so it will be closest to word */
	if ((emphasisBuffer[at].begin | emphasisBuffer[at].end | emphasisBuffer[at].word |
				emphasisBuffer[at].symbol) &
			capsEmphClass) {
		insertEmphasisBegin(emphasisBuffer, at, capsRule, capsEmphClass, table, pos,
				input, output, posMapping, cursorPosition, cursorStatus);
		insertEmphasisSymbol(emphasisBuffer, at, capsRule, capsEmphClass, table, pos,
				input, output, posMapping, cursorPosition, cursorStatus);
	}
}
