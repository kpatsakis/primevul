swapTest(int swapIC, int *pos, const TranslationTableHeader *table, const InString *input,
		const widechar *passInstructions) {
	int p = *pos;
	TranslationTableOffset swapRuleOffset;
	TranslationTableRule *swapRule;
	swapRuleOffset = (passInstructions[swapIC + 1] << 16) | passInstructions[swapIC + 2];
	swapRule = (TranslationTableRule *)&table->ruleArea[swapRuleOffset];
	while (p - *pos < passInstructions[swapIC + 3]) {
		int test;
		if (swapRule->opcode == CTO_SwapDd) {
			for (test = 1; test < swapRule->charslen; test += 2) {
				if (input->chars[p] == swapRule->charsdots[test]) break;
			}
		} else {
			for (test = 0; test < swapRule->charslen; test++) {
				if (input->chars[p] == swapRule->charsdots[test]) break;
			}
		}
		if (test >= swapRule->charslen) return 0;
		p++;
	}
	if (passInstructions[swapIC + 3] == passInstructions[swapIC + 4]) {
		*pos = p;
		return 1;
	}
	while (p - *pos < passInstructions[swapIC + 4]) {
		int test;
		if (swapRule->opcode == CTO_SwapDd) {
			for (test = 1; test < swapRule->charslen; test += 2) {
				if (input->chars[p] == swapRule->charsdots[test]) break;
			}
		} else {
			for (test = 0; test < swapRule->charslen; test++) {
				if (input->chars[p] == swapRule->charsdots[test]) break;
			}
		}
		if (test >= swapRule->charslen) {
			*pos = p;
			return 1;
		}
		p++;
	}
	*pos = p;
	return 1;
}
