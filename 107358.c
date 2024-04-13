makeCorrections(const TranslationTableHeader *table, const InString *input,
		OutString *output, int *posMapping, formtype *typebuf, int *realInlen,
		int *posIncremented, int *cursorPosition, int *cursorStatus) {
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
	if (!table->corrections) return 1;
	pos = 0;
	output->length = 0;
	*posIncremented = 1;
	_lou_resetPassVariables();
	while (pos < input->length) {
		int length = input->length - pos;
		const TranslationTableCharacter *character =
				findCharOrDots(input->chars[pos], 0, table);
		const TranslationTableCharacter *character2;
		int tryThis = 0;
		if (!findForPassRule(table, pos, 0, input, &transOpcode, &transRule,
					&transCharslen, &passCharDots, &passInstructions, &passIC,
					&patternMatch, &groupingRule, &groupingOp))
			while (tryThis < 3) {
				TranslationTableOffset ruleOffset = 0;
				unsigned long int makeHash = 0;
				switch (tryThis) {
				case 0:
					if (!(length >= 2)) break;
					makeHash = (unsigned long int)character->lowercase << 8;
					character2 = findCharOrDots(input->chars[pos + 1], 0, table);
					makeHash += (unsigned long int)character2->lowercase;
					makeHash %= HASHNUM;
					ruleOffset = table->forRules[makeHash];
					break;
				case 1:
					if (!(length >= 1)) break;
					length = 1;
					ruleOffset = character->otherRules;
					break;
				case 2: /* No rule found */
					transOpcode = CTO_Always;
					ruleOffset = 0;
					break;
				}
				while (ruleOffset) {
					transRule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
					transOpcode = transRule->opcode;
					transCharslen = transRule->charslen;
					if (tryThis == 1 || (transCharslen <= length &&
												compareChars(&transRule->charsdots[0],
														&input->chars[pos], transCharslen,
														0, table))) {
						if (*posIncremented && transOpcode == CTO_Correct &&
								passDoTest(table, pos, input, transOpcode, transRule,
										&passCharDots, &passInstructions, &passIC,
										&patternMatch, &groupingRule, &groupingOp)) {
							tryThis = 4;
							break;
						}
					}
					ruleOffset = transRule->charsnext;
				}
				tryThis++;
			}
		*posIncremented = 1;

		switch (transOpcode) {
		case CTO_Always:
			if (output->length >= output->maxlength) goto failure;
			posMapping[output->length] = pos;
			output->chars[output->length++] = input->chars[pos++];
			break;
		case CTO_Correct: {
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
		default:
			break;
		}
	}

	{  // We have to transform typebuf accordingly
		int k;
		formtype *typebuf_temp;
		if ((typebuf_temp = malloc(output->length * sizeof(formtype))) == NULL)
			_lou_outOfMemory();
		for (k = 0; k < output->length; k++)
			if (posMapping[k] < 0)
				typebuf_temp[k] = typebuf[0];  // prepend to next
			else if (posMapping[k] >= input->length)
				typebuf_temp[k] = typebuf[input->length - 1];  // append to previous
			else
				typebuf_temp[k] = typebuf[posMapping[k]];
		memcpy(typebuf, typebuf_temp, output->length * sizeof(formtype));
		free(typebuf_temp);
	}

failure:
	*realInlen = pos;
	if (input->bufferIndex != origInput->bufferIndex)
		releaseStringBuffer(input->bufferIndex);
	return 1;
}
