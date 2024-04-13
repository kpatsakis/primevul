insertEmphasisSymbol(const EmphasisInfo *buffer, const int at,
		const EmphRuleNumber emphRule, const EmphasisClass class,
		const TranslationTableHeader *table, int pos, const InString *input,
		OutString *output, int *posMapping, int *cursorPosition, int *cursorStatus) {
	if (buffer[at].symbol & class) {
		const TranslationTableRule *indicRule;
		if (brailleIndicatorDefined(
					table->emphRules[emphRule][letterOffset], table, &indicRule))
			for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
	}
}
