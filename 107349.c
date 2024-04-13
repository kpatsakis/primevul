insertEmphasisBegin(const EmphasisInfo *buffer, const int at,
		const EmphRuleNumber emphRule, const EmphasisClass class,
		const TranslationTableHeader *table, int pos, const InString *input,
		OutString *output, int *posMapping, int *cursorPosition, int *cursorStatus) {
	const TranslationTableRule *indicRule;
	if (buffer[at].begin & class) {
		if (brailleIndicatorDefined(
					table->emphRules[emphRule][begPhraseOffset], table, &indicRule))
			for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
		else if (brailleIndicatorDefined(
						 table->emphRules[emphRule][begOffset], table, &indicRule))
			for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
	}

	if (buffer[at].word & class
			&& !(buffer[at].end & class)) {
		if (brailleIndicatorDefined(
					table->emphRules[emphRule][begWordOffset], table, &indicRule))
			for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
	}
}
