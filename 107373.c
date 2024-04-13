resolveEmphasisPassages(EmphasisInfo *buffer, const EmphRuleNumber emphRule,
		const EmphasisClass class, const TranslationTableHeader *table,
		const InString *input, unsigned int *wordBuffer) {
	unsigned int word_cnt = 0;
	int pass_start = -1, pass_end = -1, word_start = -1, in_word = 0, in_pass = 0;
	int i;

	for (i = 0; i < input->length; i++) {
		/* check if at beginning of word */
		if (!in_word)
			if (wordBuffer[i] & WORD_CHAR) {
				in_word = 1;
				if (wordBuffer[i] & WORD_WHOLE) {
					if (!in_pass) {
						in_pass = 1;
						pass_start = i;
						pass_end = -1;
						word_cnt = 1;
					} else
						word_cnt++;
					word_start = i;
					continue;
				} else if (in_pass) {
					if (word_cnt >= table->emphRules[emphRule][lenPhraseOffset])
						if (pass_end >= 0) {
							convertToPassage(pass_start, pass_end, word_start, buffer,
									emphRule, class, table, wordBuffer);
						}
					in_pass = 0;
				}
			}

		/* check if at end of word */
		if (in_word)
			if (!(wordBuffer[i] & WORD_CHAR)) {
				in_word = 0;
				if (in_pass) pass_end = i;
			}

		if (in_pass)
			if ((buffer[i].begin | buffer[i].end | buffer[i].word | buffer[i].symbol) &
					class) {
				if (word_cnt >= table->emphRules[emphRule][lenPhraseOffset])
					if (pass_end >= 0) {
						convertToPassage(pass_start, pass_end, word_start, buffer,
								emphRule, class, table, wordBuffer);
					}
				in_pass = 0;
			}
	}

	if (in_pass) {
		if (word_cnt >= table->emphRules[emphRule][lenPhraseOffset]) {
			if (pass_end >= 0) {
				if (in_word) {
					convertToPassage(pass_start, i, word_start, buffer, emphRule, class,
							table, wordBuffer);
				} else {
					convertToPassage(pass_start, pass_end, word_start, buffer, emphRule,
							class, table, wordBuffer);
				}
			}
		}
	}
}
