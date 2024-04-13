translatePass(const TranslationTableHeader *table, int currentPass, const InString *input,
		OutString *output, int *posMapping, int *realInlen, int *posIncremented,
		int *cursorPosition, int *cursorStatus) {
	int pos;
	int transOpcode;
	const TranslationTableRule *transRule;
	int transCharslen;
	int passCharDots;
	const widechar *passInstructions;
	int passIC; /* Instruction counter */
	PassRuleMatch patternMatch;
	TranslationTableRule *groupingRule;
	widechar groupingOp;
	const InString *origInput = input;
	pos = output->length = 0;
	*posIncremented = 1;
	_lou_resetPassVariables();
	while (pos < input->length) { /* the main multipass translation loop */
		passSelectRule(table, pos, currentPass, input, &transOpcode, &transRule,
				&transCharslen, &passCharDots, &passInstructions, &passIC, &patternMatch,
				&groupingRule, &groupingOp);
		*posIncremented = 1;
		switch (transOpcode) {
		case CTO_Context:
		case CTO_Pass2:
		case CTO_Pass3:
		case CTO_Pass4: {
			const InString *inputBefore = input;
			int posBefore = pos;
			if (appliedRules != NULL && appliedRulesCount < maxAppliedRules)
				appliedRules[appliedRulesCount++] = transRule;
			if (!passDoAction(table, &input, output, posMapping, transOpcode, &transRule,
						passCharDots, passInstructions, passIC, &pos, patternMatch,
						cursorPosition, cursorStatus, groupingRule, groupingOp))
				goto failure;
			if (input->bufferIndex != inputBefore->bufferIndex &&
					inputBefore->bufferIndex != origInput->bufferIndex)
				releaseStringBuffer(inputBefore->bufferIndex);
			if (pos == posBefore) *posIncremented = 0;
			break;
		}
		case CTO_Always:
			if ((output->length + 1) > output->maxlength) goto failure;
			posMapping[output->length] = pos;
			output->chars[output->length++] = input->chars[pos++];
			break;
		default:
			goto failure;
		}
	}
failure:
	if (pos < input->length) {
		while (checkAttr(input->chars[pos], CTC_Space, 1, table))
			if (++pos == input->length) break;
	}
	*realInlen = pos;
	if (input->bufferIndex != origInput->bufferIndex)
		releaseStringBuffer(input->bufferIndex);
	return 1;
}
