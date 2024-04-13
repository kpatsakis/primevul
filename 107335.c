doPassSearch(const TranslationTableHeader *table, const InString *input,
		const TranslationTableRule *transRule, int passCharDots, int pos,
		const widechar *passInstructions, int passIC, int *searchIC, int *searchPos,
		TranslationTableRule *groupingRule, widechar groupingOp) {
	int level = 0;
	int k, kk;
	int not = 0;  // whether next operand should be reversed
	TranslationTableOffset ruleOffset;
	TranslationTableRule *rule;
	TranslationTableCharacterAttributes attributes;
	while (pos < input->length) {
		*searchIC = passIC + 1;
		*searchPos = pos;
		while (*searchIC < transRule->dotslen) {
			int itsTrue = 1;  // whether we have a match or not
			if (*searchPos > input->length) return 0;
			switch (passInstructions[*searchIC]) {
			case pass_lookback:
				*searchPos -= passInstructions[*searchIC + 1];
				if (*searchPos < 0) {
					*searchPos = 0;
					itsTrue = 0;
				}
				*searchIC += 2;
				break;
			case pass_not:
				not = !not;
				(*searchIC)++;
				continue;
			case pass_string:
			case pass_dots:
				kk = *searchPos;
				for (k = *searchIC + 2;
						k < *searchIC + 2 + passInstructions[*searchIC + 1]; k++)
					if (input->chars[kk] == ENDSEGMENT ||
							passInstructions[k] != input->chars[kk++]) {
						itsTrue = 0;
						break;
					}
				*searchPos += passInstructions[*searchIC + 1];
				*searchIC += passInstructions[*searchIC + 1] + 2;
				break;
			case pass_startReplace:
				(*searchIC)++;
				break;
			case pass_endReplace:
				(*searchIC)++;
				break;
			case pass_attributes:
				attributes = (passInstructions[*searchIC + 1] << 16) |
						passInstructions[*searchIC + 2];
				for (k = 0; k < passInstructions[*searchIC + 3]; k++) {
					if (input->chars[*searchPos] == ENDSEGMENT)
						itsTrue = 0;
					else {
						itsTrue = ((findCharOrDots(input->chars[(*searchPos)++],
											passCharDots,
											table)->attributes &
										   attributes)
										? 1
										: 0);
						if (not) itsTrue = !itsTrue;
					}
					if (!itsTrue) break;
				}
				if (itsTrue) {
					for (k = passInstructions[*searchIC + 3];
							k < passInstructions[*searchIC + 4]; k++) {
						if (input->chars[*searchPos] == ENDSEGMENT) {
							itsTrue = 0;
							break;
						}
						if (!(findCharOrDots(input->chars[*searchPos], passCharDots,
									  table)->attributes &
									attributes)) {
							if (!not) break;
						} else if (not)
							break;
						(*searchPos)++;
					}
				}
				not = 0;
				*searchIC += 5;
				break;
			case pass_groupstart:
			case pass_groupend:
				ruleOffset = (passInstructions[*searchIC + 1] << 16) |
						passInstructions[*searchIC + 2];
				rule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
				if (passInstructions[*searchIC] == pass_groupstart)
					itsTrue = (input->chars[*searchPos] ==
									  rule->charsdots[2 * passCharDots])
							? 1
							: 0;
				else
					itsTrue = (input->chars[*searchPos] ==
									  rule->charsdots[2 * passCharDots + 1])
							? 1
							: 0;
				if (groupingRule != NULL && groupingOp == pass_groupstart &&
						rule == groupingRule) {
					if (input->chars[*searchPos] == rule->charsdots[2 * passCharDots])
						level--;
					else if (input->chars[*searchPos] ==
							rule->charsdots[2 * passCharDots + 1])
						level++;
				}
				(*searchPos)++;
				*searchIC += 3;
				break;
			case pass_swap:
				itsTrue = swapTest(*searchIC, searchPos, table, input, passInstructions);
				*searchIC += 5;
				break;
			case pass_endTest:
				if (itsTrue) {
					if ((groupingRule && level == 1) || !groupingRule) return 1;
				}
				*searchIC = transRule->dotslen;
				break;
			default:
				if (_lou_handlePassVariableTest(passInstructions, searchIC, &itsTrue))
					break;
				break;
			}
			if ((!not&&!itsTrue) || (not&&itsTrue)) break;
			not = 0;
		}
		pos++;
	}
	return 0;
}
