passDoAction(const TranslationTableHeader *table, const InString **input,
		OutString *output, int *posMapping, int transOpcode,
		const TranslationTableRule **transRule, int passCharDots,
		const widechar *passInstructions, int passIC, int *pos, PassRuleMatch match,
		int *cursorPosition, int *cursorStatus, TranslationTableRule *groupingRule,
		widechar groupingOp) {
	int k;
	TranslationTableOffset ruleOffset = 0;
	TranslationTableRule *rule = NULL;
	int destStartMatch = output->length;
	int destStartReplace;
	int newPos = match.endReplace;

	if (!copyCharacters(match.startMatch, match.startReplace, table, *input, output,
				posMapping, transOpcode, cursorPosition, cursorStatus))
		return 0;
	destStartReplace = output->length;

	while (passIC < (*transRule)->dotslen) switch (passInstructions[passIC]) {
		case pass_string:
		case pass_dots:
			if ((output->length + passInstructions[passIC + 1]) > output->maxlength)
				return 0;
			for (k = 0; k < passInstructions[passIC + 1]; ++k)
				posMapping[output->length + k] = match.startReplace;
			memcpy(&output->chars[output->length], &passInstructions[passIC + 2],
					passInstructions[passIC + 1] * CHARSIZE);
			output->length += passInstructions[passIC + 1];
			passIC += passInstructions[passIC + 1] + 2;
			break;
		case pass_groupstart:
			ruleOffset =
					(passInstructions[passIC + 1] << 16) | passInstructions[passIC + 2];
			rule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
			posMapping[output->length] = match.startMatch;
			output->chars[output->length++] = rule->charsdots[2 * passCharDots];
			passIC += 3;
			break;
		case pass_groupend:
			ruleOffset =
					(passInstructions[passIC + 1] << 16) | passInstructions[passIC + 2];
			rule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
			posMapping[output->length] = match.startMatch;
			output->chars[output->length++] = rule->charsdots[2 * passCharDots + 1];
			passIC += 3;
			break;
		case pass_swap:
			if (!swapReplace(match.startReplace, match.endReplace, table, *input, output,
						posMapping, passInstructions, passIC))
				return 0;
			passIC += 3;
			break;
		case pass_groupreplace:
			if (!groupingRule ||
					!replaceGrouping(table, input, output, transOpcode, passCharDots,
							passInstructions, passIC, match.startReplace, groupingRule,
							groupingOp))
				return 0;
			passIC += 3;
			break;
		case pass_omit:
			if (groupingRule)
				removeGrouping(input, output, passCharDots, match.startReplace,
						groupingRule, groupingOp);
			passIC++;
			break;
		case pass_copy: {
			int count = destStartReplace - destStartMatch;
			if (count > 0) {
				memmove(&output->chars[destStartMatch], &output->chars[destStartReplace],
						count * sizeof(*output->chars));
				output->length -= count;
				destStartReplace = destStartMatch;
			}
		}

			if (!copyCharacters(match.startReplace, match.endReplace, table, *input,
						output, posMapping, transOpcode, cursorPosition, cursorStatus))
				return 0;
			newPos = match.endMatch;
			passIC++;
			break;
		default:
			if (_lou_handlePassVariableAction(passInstructions, &passIC)) break;
			return 0;
		}
	*pos = newPos;
	return 1;
}
