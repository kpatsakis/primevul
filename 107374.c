resolveEmphasisResets(EmphasisInfo *buffer, const EmphasisClass class,
		const TranslationTableHeader *table, const InString *input,
		unsigned int *wordBuffer) {
	int in_word = 0, in_pass = 0, word_start = -1, word_reset = 0, orig_reset = -1,
		letter_cnt = 0;
	int i, j;

	for (i = 0; i < input->length; i++) {
		if (in_pass)
			if (buffer[i].end & class) in_pass = 0;

		if (!in_pass) {
			if (buffer[i].begin & class)
				in_pass = 1;
			else {
				if (!in_word) {
					if (buffer[i].word & class) {
						/* deal with case when reset
						 * was at beginning of word */
						if (wordBuffer[i] & WORD_RESET ||
								!checkAttr(input->chars[i], CTC_Letter, 0, table)) {
							/* not just one reset by itself */
							if (wordBuffer[i + 1] & WORD_CHAR) {
								buffer[i + 1].word |= class;
								if (wordBuffer[i] & WORD_WHOLE)
									wordBuffer[i + 1] |= WORD_WHOLE;
							}
							buffer[i].word &= ~class;
							wordBuffer[i] &= ~WORD_WHOLE;

							/* if reset is a letter, make it a symbol */
							if (checkAttr(input->chars[i], CTC_Letter, 0, table))
								buffer[i].symbol |= class;

							continue;
						}

						in_word = 1;
						word_start = i;
						letter_cnt = 0;
						word_reset = 0;
					}

					/* it is possible for a character to have been
					 * marked as a symbol when it should not be one */
					else if (buffer[i].symbol & class) {
						if (wordBuffer[i] & WORD_RESET ||
								!checkAttr(input->chars[i], CTC_Letter, 0, table))
							buffer[i].symbol &= ~class;
					}
				}

				if (in_word) {

					/* at end of word */
					if (!(wordBuffer[i] & WORD_CHAR) ||
							(buffer[i].word & class && buffer[i].end & class)) {
						in_word = 0;

						/* check if symbol */
						if (letter_cnt == 1) {
							buffer[word_start].symbol |= class;
							buffer[word_start].word &= ~class;
							wordBuffer[word_start] &= ~WORD_WHOLE;
							buffer[i].end &= ~class;
							buffer[i].word &= ~class;
						}

						/* if word ended on a reset or last char was a reset,
						 * get rid of end bits */
						if (word_reset || wordBuffer[i] & WORD_RESET ||
								!checkAttr(input->chars[i], CTC_Letter, 0, table)) {
							buffer[i].end &= ~class;
							buffer[i].word &= ~class;
						}

						/* if word ended when it began, get rid of all bits */
						if (i == word_start) {
							wordBuffer[word_start] &= ~WORD_WHOLE;
							buffer[i].end &= ~class;
							buffer[i].word &= ~class;
						}
						orig_reset = -1;
					} else {
						/* hit reset */
						if (wordBuffer[i] & WORD_RESET ||
								!checkAttr(input->chars[i], CTC_Letter, 0, table)) {
							if (!checkAttr(input->chars[i], CTC_Letter, 0, table)) {
								if (checkAttr(input->chars[i], CTC_CapsMode, 0, table)) {
									/* chars marked as not resetting */
									orig_reset = i;
									continue;
								} else if (orig_reset >= 0) {
									/* invalid no reset sequence */
									for (j = orig_reset; j < i; j++)
										buffer[j].word &= ~class;
									orig_reset = -1;
								}
							}

							/* check if symbol is not already resetting */
							if (letter_cnt == 1) {
								buffer[word_start].symbol |= class;
								buffer[word_start].word &= ~class;
								wordBuffer[word_start] &= ~WORD_WHOLE;
							}

							/* if reset is a letter, make it the new word_start */
							if (checkAttr(input->chars[i], CTC_Letter, 0, table)) {
								word_reset = 0;
								word_start = i;
								letter_cnt = 1;
								buffer[i].word |= class;
							} else
								word_reset = 1;

							continue;
						}

						if (word_reset) {
							word_reset = 0;
							word_start = i;
							letter_cnt = 0;
							buffer[i].word |= class;
						}

						letter_cnt++;
					}
				}
			}
		}
	}

	/* clean up end */
	if (in_word) {
		/* check if symbol */
		if (letter_cnt == 1) {
			buffer[word_start].symbol |= class;
			buffer[word_start].word &= ~class;
			wordBuffer[word_start] &= ~WORD_WHOLE;
			buffer[i].end &= ~class;
			buffer[i].word &= ~class;
		}

		if (word_reset) {
			buffer[i].end &= ~class;
			buffer[i].word &= ~class;
		}
	}
}
