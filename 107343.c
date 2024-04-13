inSequence(const TranslationTableHeader *table, int pos, const InString *input,
		const TranslationTableRule *transRule) {
	int i, j, s, match;

	/* check before sequence */
	for (i = pos - 1; i >= 0; i--) {
		if (checkAttr(input->chars[i], CTC_SeqBefore, 0, table)) continue;
		if (!(checkAttr(input->chars[i], CTC_Space | CTC_SeqDelimiter, 0, table)))
			return 0;
		break;
	}

	/* check after sequence */
	for (i = pos + transRule->charslen; i < input->length; i++) {
		/* check sequence after patterns */
		if (table->seqPatternsCount) {
			match = 0;
			for (j = i, s = 0; j <= input->length && s < table->seqPatternsCount;
					j++, s++) {
				/* matching */
				if (match == 1) {
					if (table->seqPatterns[s]) {
						if (input->chars[j] == table->seqPatterns[s])
							match = 1;
						else {
							match = -1;
							j = i - 1;
						}
					}

					/* found match */
					else {
						/* pattern at end of input */
						if (j >= input->length) return 1;

						i = j;
						break;
					}
				}

				/* looking for match */
				else if (match == 0) {
					if (table->seqPatterns[s]) {
						if (input->chars[j] == table->seqPatterns[s])
							match = 1;
						else {
							match = -1;
							j = i - 1;
						}
					}
				}

				/* next pattarn */
				else if (match == -1) {
					if (!table->seqPatterns[s]) {
						match = 0;
						j = i - 1;
					}
				}
			}
		}

		if (checkAttr(input->chars[i], CTC_SeqAfter, 0, table)) continue;
		if (!(checkAttr(input->chars[i], CTC_Space | CTC_SeqDelimiter, 0, table)))
			return 0;
		break;
	}

	return 1;
}
