swapReplace(int start, int end, const TranslationTableHeader *table,
		const InString *input, OutString *output, int *posMapping,
		const widechar *passInstructions, int passIC) {
	TranslationTableOffset swapRuleOffset;
	TranslationTableRule *swapRule;
	widechar *replacements;
	int p;
	swapRuleOffset = (passInstructions[passIC + 1] << 16) | passInstructions[passIC + 2];
	swapRule = (TranslationTableRule *)&table->ruleArea[swapRuleOffset];
	replacements = &swapRule->charsdots[swapRule->charslen];
	for (p = start; p < end; p++) {
		int rep;
		int test;
		int k;
		if (swapRule->opcode == CTO_SwapDd) {
			for (test = 0; test * 2 + 1 < swapRule->charslen; test++)
				if (input->chars[p] == swapRule->charsdots[test * 2 + 1]) break;
			if (test * 2 == swapRule->charslen) continue;
		} else {
			for (test = 0; test < swapRule->charslen; test++)
				if (input->chars[p] == swapRule->charsdots[test]) break;
			if (test == swapRule->charslen) continue;
		}
		k = 0;
		for (rep = 0; rep < test; rep++)
			if (swapRule->opcode == CTO_SwapCc)
				k++;
			else
				k += replacements[k];
		if (swapRule->opcode == CTO_SwapCc) {
			if ((output->length + 1) > output->maxlength) return 0;
			posMapping[output->length] = p;
			output->chars[output->length++] = replacements[k];
		} else {
			int l = replacements[k] - 1;
			int d = output->length + l;
			if (d > output->maxlength) return 0;
			while (--d >= output->length) posMapping[d] = p;
			memcpy(&output->chars[output->length], &replacements[k + 1],
					l * sizeof(*output->chars));
			output->length += l;
		}
	}
	return 1;
}
