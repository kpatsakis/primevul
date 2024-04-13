insertEmphasisEnd(const EmphasisInfo *buffer, const int at, const EmphRuleNumber emphRule,
		const EmphasisClass class, const TranslationTableHeader *table, int pos,
		const InString *input, OutString *output, int *posMapping, int *cursorPosition,
		int *cursorStatus) {
	if (buffer[at].end & class) {
		const TranslationTableRule *indicRule;
		if (buffer[at].word & class) {
			if (brailleIndicatorDefined(
						table->emphRules[emphRule][endWordOffset], table, &indicRule))
				for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, -1,
						pos, input, output, posMapping, cursorPosition, cursorStatus);
		} else {
			if (brailleIndicatorDefined(
						table->emphRules[emphRule][endOffset], table, &indicRule))
				for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, -1,
						pos, input, output, posMapping, cursorPosition, cursorStatus);
			else if (brailleIndicatorDefined(
							 table->emphRules[emphRule][endPhraseAfterOffset], table,
							 &indicRule))
				for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, -1,
						pos, input, output, posMapping, cursorPosition, cursorStatus);
			else if (brailleIndicatorDefined(
							 table->emphRules[emphRule][endPhraseBeforeOffset], table,
							 &indicRule))
				for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0,
						pos, input, output, posMapping, cursorPosition, cursorStatus);
		}
	}
}
