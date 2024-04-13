markSyllables(const TranslationTableHeader *table, const InString *input,
		formtype *typebuf, int *transOpcode, const TranslationTableRule **transRule,
		int *transCharslen) {
	int pos;
	int k;
	int currentMark = 0;
	int const syllable_marks[] = { SYLLABLE_MARKER_1, SYLLABLE_MARKER_2 };
	int syllable_mark_selector = 0;

	if (typebuf == NULL || !table->syllables) return 1;
	pos = 0;
	while (pos < input->length) { /* the main multipass translation loop */
		int length = input->length - pos;
		const TranslationTableCharacter *character =
				findCharOrDots(input->chars[pos], 0, table);
		const TranslationTableCharacter *character2;
		int tryThis = 0;
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
				*transOpcode = CTO_Always;
				ruleOffset = 0;
				break;
			}
			while (ruleOffset) {
				*transRule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
				*transOpcode = (*transRule)->opcode;
				*transCharslen = (*transRule)->charslen;
				if (tryThis == 1 ||
						(*transCharslen <= length &&
								compareChars(&(*transRule)->charsdots[0],
										&input->chars[pos], *transCharslen, 0, table))) {
					if (*transOpcode == CTO_Syllable) {
						tryThis = 4;
						break;
					}
				}
				ruleOffset = (*transRule)->charsnext;
			}
			tryThis++;
		}
		switch (*transOpcode) {
		case CTO_Always:
			if (pos >= input->length) return 0;
			typebuf[pos++] |= currentMark;
			break;
		case CTO_Syllable:
			/* cycle between SYLLABLE_MARKER_1 and SYLLABLE_MARKER_2 so
			 * we can distinguinsh two consequtive syllables */
			currentMark = syllable_marks[syllable_mark_selector];
			syllable_mark_selector = (syllable_mark_selector + 1) % 2;

			if ((pos + *transCharslen) > input->length) return 0;
			for (k = 0; k < *transCharslen; k++) typebuf[pos++] |= currentMark;
			break;
		default:
			break;
		}
	}
	return 1;
}
