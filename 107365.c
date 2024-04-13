passDoTest(const TranslationTableHeader *table, int pos, const InString *input,
		int transOpcode, const TranslationTableRule *transRule, int *passCharDots,
		widechar const **passInstructions, int *passIC, PassRuleMatch *match,
		TranslationTableRule **groupingRule, widechar *groupingOp) {
	int searchIC, searchPos;
	int k;
	int not = 0;  // whether next operand should be reversed
	TranslationTableOffset ruleOffset = 0;
	TranslationTableRule *rule = NULL;
	TranslationTableCharacterAttributes attributes = 0;
	int startMatch = pos;
	int endMatch = pos;
	int startReplace = -1;
	int endReplace = -1;
	*groupingRule = NULL;
	*passInstructions = &transRule->charsdots[transRule->charslen];
	*passIC = 0;
	if (transOpcode == CTO_Context || transOpcode == CTO_Correct)
		*passCharDots = 0;
	else
		*passCharDots = 1;
	while (*passIC < transRule->dotslen) {
		int itsTrue = 1;  // whether we have a match or not
		if (pos > input->length) return 0;
		switch ((*passInstructions)[*passIC]) {
		case pass_first:
			if (pos != 0) itsTrue = 0;
			(*passIC)++;
			break;
		case pass_last:
			if (pos != input->length) itsTrue = 0;
			(*passIC)++;
			break;
		case pass_lookback:
			pos -= (*passInstructions)[*passIC + 1];
			if (pos < 0) {
				searchPos = 0;
				itsTrue = 0;
			}
			*passIC += 2;
			break;
		case pass_not:
			not = !not;
			(*passIC)++;
			continue;
		case pass_string:
		case pass_dots:
			itsTrue = matchCurrentInput(input, pos, *passInstructions, *passIC);
			pos += (*passInstructions)[*passIC + 1];
			*passIC += (*passInstructions)[*passIC + 1] + 2;
			break;
		case pass_startReplace:
			startReplace = pos;
			(*passIC)++;
			break;
		case pass_endReplace:
			endReplace = pos;
			(*passIC)++;
			break;
		case pass_attributes:
			attributes = ((*passInstructions)[*passIC + 1] << 16) |
					(*passInstructions)[*passIC + 2];
			for (k = 0; k < (*passInstructions)[*passIC + 3]; k++) {
				if (pos >= input->length) {
					itsTrue = 0;
					break;
				}
				if (input->chars[pos] == ENDSEGMENT) {
					itsTrue = 0;
					break;
				}
				if (!(findCharOrDots(input->chars[pos], *passCharDots,
							  table)->attributes &
							attributes)) {
					if (!not) {
						itsTrue = 0;
						break;
					}
				} else if (not) {
					itsTrue = 0;
					break;
				}
				pos++;
			}
			if (itsTrue) {
				for (k = (*passInstructions)[*passIC + 3];
						k < (*passInstructions)[*passIC + 4] && pos < input->length;
						k++) {
					if (input->chars[pos] == ENDSEGMENT) {
						itsTrue = 0;
						break;
					}
					if (!(findCharOrDots(input->chars[pos], *passCharDots,
								  table)->attributes &
								attributes)) {
						if (!not) break;
					} else if (not)
						break;
					pos++;
				}
			}
			not = 0;
			*passIC += 5;
			break;
		case pass_groupstart:
		case pass_groupend:
			ruleOffset = ((*passInstructions)[*passIC + 1] << 16) |
					(*passInstructions)[*passIC + 2];
			rule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
			if (*passIC == 0 ||
					(*passIC > 0 &&
							(*passInstructions)[*passIC - 1] == pass_startReplace)) {
				*groupingRule = rule;
				*groupingOp = (*passInstructions)[*passIC];
			}
			if ((*passInstructions)[*passIC] == pass_groupstart)
				itsTrue =
						(input->chars[pos] == rule->charsdots[2 * *passCharDots]) ? 1 : 0;
			else
				itsTrue = (input->chars[pos] == rule->charsdots[2 * *passCharDots + 1])
						? 1
						: 0;
			pos++;
			*passIC += 3;
			break;
		case pass_swap:
			itsTrue = swapTest(*passIC, &pos, table, input, *passInstructions);
			*passIC += 5;
			break;
		case pass_search:
			itsTrue = doPassSearch(table, input, transRule, *passCharDots, pos,
					*passInstructions, *passIC, &searchIC, &searchPos, *groupingRule,
					*groupingOp);
			if ((!not&&!itsTrue) || (not&&itsTrue)) return 0;
			*passIC = searchIC;
			pos = searchPos;
		case pass_endTest:
			(*passIC)++;
			endMatch = pos;
			if (startReplace == -1) {
				startReplace = startMatch;
				endReplace = endMatch;
			}
			*match = (PassRuleMatch){.startMatch = startMatch,
				.startReplace = startReplace,
				.endReplace = endReplace,
				.endMatch = endMatch };
			return 1;
			break;
		default:
			if (_lou_handlePassVariableTest(*passInstructions, passIC, &itsTrue)) break;
			return 0;
		}
		if ((!not&&!itsTrue) || (not&&itsTrue)) return 0;
		not = 0;
	}
	return 0;
}
