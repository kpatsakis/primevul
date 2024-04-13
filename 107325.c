brailleIndicatorDefined(TranslationTableOffset offset,
		const TranslationTableHeader *table, const TranslationTableRule **indicRule) {
	if (!offset) return 0;
	*indicRule = (TranslationTableRule *)&table->ruleArea[offset];
	return 1;
}
