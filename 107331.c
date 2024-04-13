convertToPassage(const int pass_start, const int pass_end, const int word_start,
		EmphasisInfo *buffer, const EmphRuleNumber emphRule, const EmphasisClass class,
		const TranslationTableHeader *table, unsigned int *wordBuffer) {
	int i;
	const TranslationTableRule *indicRule;

	for (i = pass_start; i <= pass_end; i++)
		if (wordBuffer[i] & WORD_WHOLE) {
			buffer[i].symbol &= ~class;
			buffer[i].word &= ~class;
			wordBuffer[i] &= ~WORD_WHOLE;
		}

	buffer[pass_start].begin |= class;
	if (brailleIndicatorDefined(
				table->emphRules[emphRule][endOffset], table, &indicRule) ||
			brailleIndicatorDefined(
					table->emphRules[emphRule][endPhraseAfterOffset], table, &indicRule))
		buffer[pass_end].end |= class;
	else if (brailleIndicatorDefined(table->emphRules[emphRule][endPhraseBeforeOffset],
					 table, &indicRule))
		buffer[word_start].end |= class;
}
