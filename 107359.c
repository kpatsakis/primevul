markEmphases(const TranslationTableHeader *table, const InString *input,
		formtype *typebuf, unsigned int *wordBuffer, EmphasisInfo *emphasisBuffer,
		int haveEmphasis) {
	/* Relies on the order of typeforms emph_1..emph_10. */
	int caps_start = -1, last_caps = -1, caps_cnt = 0;
	int emph_start[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int i, j;

	if (haveEmphasis && !emphClasses) {
		initEmphClasses();
	}

	for (i = 0; i < input->length; i++) {
		if (!checkAttr(input->chars[i], CTC_Space, 0, table)) {
			wordBuffer[i] |= WORD_CHAR;
		} else if (caps_cnt > 0) {
			last_caps = i;
			caps_cnt = 0;
		}

		if (checkAttr(input->chars[i], CTC_UpperCase, 0, table)) {
			if (caps_start < 0) caps_start = i;
			caps_cnt++;
		} else if (caps_start >= 0) {
			/* caps should keep going until this */
			if (checkAttr(input->chars[i], CTC_Letter, 0, table) &&
					checkAttr(input->chars[i], CTC_LowerCase, 0, table)) {
				emphasisBuffer[caps_start].begin |= capsEmphClass;
				if (caps_cnt > 0)
					emphasisBuffer[i].end |= capsEmphClass;
				else
					emphasisBuffer[last_caps].end |= capsEmphClass;
				caps_start = -1;
				last_caps = -1;
				caps_cnt = 0;
			}
		}

		if (!haveEmphasis) continue;

		for (j = 0; j < 10; j++) {
			if (typebuf[i] & (emph_1 << j)) {
				if (emph_start[j] < 0) emph_start[j] = i;
			} else if (emph_start[j] >= 0) {
				emphasisBuffer[emph_start[j]].begin |= emphClasses[j];
				emphasisBuffer[i].end |= emphClasses[j];
				emph_start[j] = -1;
			}
		}
	}

	/* clean up input->length */
	if (caps_start >= 0) {
		emphasisBuffer[caps_start].begin |= capsEmphClass;
		if (caps_cnt > 0)
			emphasisBuffer[input->length].end |= capsEmphClass;
		else
			emphasisBuffer[last_caps].end |= capsEmphClass;
	}

	if (haveEmphasis) {
		for (j = 0; j < 10; j++) {
			if (emph_start[j] >= 0) {
				emphasisBuffer[emph_start[j]].begin |= emphClasses[j];
				emphasisBuffer[input->length].end |= emphClasses[j];
			}
		}
	}

	/* Handle capsnocont */
	if (table->capsNoCont) {
		int inCaps = 0;
		for (i = 0; i < input->length; i++) {
			if (emphasisBuffer[i].end & capsEmphClass) {
				inCaps = 0;
			} else {
				if ((emphasisBuffer[i].begin & capsEmphClass) &&
						!(emphasisBuffer[i + 1].end & capsEmphClass))
					inCaps = 1;
				if (inCaps) typebuf[i] |= no_contract;
			}
		}
	}
	if (table->emphRules[capsRule][begWordOffset]) {
		resolveEmphasisWords(emphasisBuffer, capsEmphClass, input, wordBuffer);
		if (table->emphRules[capsRule][lenPhraseOffset])
			resolveEmphasisPassages(
					emphasisBuffer, capsRule, capsEmphClass, table, input, wordBuffer);
		resolveEmphasisResets(emphasisBuffer, capsEmphClass, table, input, wordBuffer);
	} else if (table->emphRules[capsRule][letterOffset]) {
		if (table->capsNoCont) /* capsnocont and no capsword */
			resolveEmphasisAllCapsSymbols(emphasisBuffer, typebuf, input);
		else
			resolveEmphasisSingleSymbols(emphasisBuffer, capsEmphClass, input);
	}
	if (!haveEmphasis) return;

	for (j = 0; j < 10; j++) {
		if (table->emphRules[emph1Rule + j][begWordOffset]) {
			resolveEmphasisWords(emphasisBuffer, emphClasses[j], input, wordBuffer);
			if (table->emphRules[emph1Rule + j][lenPhraseOffset])
				resolveEmphasisPassages(emphasisBuffer, emph1Rule + j, emphClasses[j],
						table, input, wordBuffer);
		} else if (table->emphRules[emph1Rule + j][letterOffset])
			resolveEmphasisSingleSymbols(emphasisBuffer, emphClasses[j], input);
	}
}
