findForPassRule(const TranslationTableHeader *table, int pos, int currentPass,
		const InString *input, int *transOpcode, const TranslationTableRule **transRule,
		int *transCharslen, int *passCharDots, widechar const **passInstructions,
		int *passIC, PassRuleMatch *match, TranslationTableRule **groupingRule,
		widechar *groupingOp) {
	int save_transCharslen = *transCharslen;
	const TranslationTableRule *save_transRule = *transRule;
	TranslationTableOpcode save_transOpcode = *transOpcode;
	TranslationTableOffset ruleOffset;
	ruleOffset = table->forPassRules[currentPass];
	*transCharslen = 0;
	while (ruleOffset) {
		*transRule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
		*transOpcode = (*transRule)->opcode;
		if (passDoTest(table, pos, input, *transOpcode, *transRule, passCharDots,
					passInstructions, passIC, match, groupingRule, groupingOp))
			return 1;
		ruleOffset = (*transRule)->charsnext;
	}
	*transCharslen = save_transCharslen;
	*transRule = save_transRule;
	*transOpcode = save_transOpcode;
	return 0;
}
