passSelectRule(const TranslationTableHeader *table, int pos, int currentPass,
		const InString *input, int *transOpcode, const TranslationTableRule **transRule,
		int *transCharslen, int *passCharDots, widechar const **passInstructions,
		int *passIC, PassRuleMatch *match, TranslationTableRule **groupingRule,
		widechar *groupingOp) {
	if (!findForPassRule(table, pos, currentPass, input, transOpcode, transRule,
				transCharslen, passCharDots, passInstructions, passIC, match,
				groupingRule, groupingOp)) {
		*transOpcode = CTO_Always;
	}
}
