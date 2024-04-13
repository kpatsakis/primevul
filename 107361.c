noCompbrlAhead(const TranslationTableHeader *table, int pos, int mode,
		const InString *input, int transOpcode, int transCharslen, int cursorPosition) {
	int start = pos + transCharslen;
	int end;
	int p;
	if (start >= input->length) return 1;
	while (start < input->length && checkAttr(input->chars[start], CTC_Space, 0, table))
		start++;
	if (start == input->length ||
			(transOpcode == CTO_JoinableWord &&
					(!checkAttr(input->chars[start], CTC_Letter | CTC_Digit, 0, table) ||
							!checkAttr(input->chars[start - 1], CTC_Space, 0, table))))
		return 1;
	end = start;
	while (end < input->length && !checkAttr(input->chars[end], CTC_Space, 0, table))
		end++;
	if ((mode & (compbrlAtCursor | compbrlLeftCursor)) && cursorPosition >= start &&
			cursorPosition < end)
		return 0;
	/* Look ahead for rules with CTO_CompBrl */
	for (p = start; p < end; p++) {
		int length = input->length - p;
		int tryThis;
		const TranslationTableCharacter *character1;
		const TranslationTableCharacter *character2;
		int k;
		character1 = findCharOrDots(input->chars[p], 0, table);
		for (tryThis = 0; tryThis < 2; tryThis++) {
			TranslationTableOffset ruleOffset = 0;
			TranslationTableRule *testRule;
			unsigned long int makeHash = 0;
			switch (tryThis) {
			case 0:
				if (!(length >= 2)) break;
				/* Hash function optimized for forward translation */
				makeHash = (unsigned long int)character1->lowercase << 8;
				character2 = findCharOrDots(input->chars[p + 1], 0, table);
				makeHash += (unsigned long int)character2->lowercase;
				makeHash %= HASHNUM;
				ruleOffset = table->forRules[makeHash];
				break;
			case 1:
				if (!(length >= 1)) break;
				length = 1;
				ruleOffset = character1->otherRules;
				break;
			}
			while (ruleOffset) {
				testRule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
				for (k = 0; k < testRule->charslen; k++) {
					character1 = findCharOrDots(testRule->charsdots[k], 0, table);
					character2 = findCharOrDots(input->chars[p + k], 0, table);
					if (character1->lowercase != character2->lowercase) break;
				}
				if (tryThis == 1 || k == testRule->charslen) {
					if (testRule->opcode == CTO_CompBrl ||
							testRule->opcode == CTO_Literal)
						return 0;
				}
				ruleOffset = testRule->charsnext;
			}
		}
	}
	return 1;
}
