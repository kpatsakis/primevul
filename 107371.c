replaceGrouping(const TranslationTableHeader *table, const InString **input,
		OutString *output, int transOpcode, int passCharDots,
		const widechar *passInstructions, int passIC, int startReplace,
		TranslationTableRule *groupingRule, widechar groupingOp) {
	widechar startCharDots = groupingRule->charsdots[2 * passCharDots];
	widechar endCharDots = groupingRule->charsdots[2 * passCharDots + 1];
	int p;
	int level = 0;
	TranslationTableOffset replaceOffset =
			passInstructions[passIC + 1] << 16 | (passInstructions[passIC + 2] & 0xff);
	TranslationTableRule *replaceRule =
			(TranslationTableRule *)&table->ruleArea[replaceOffset];
	widechar replaceStart = replaceRule->charsdots[2 * passCharDots];
	widechar replaceEnd = replaceRule->charsdots[2 * passCharDots + 1];
	if (groupingOp == pass_groupstart) {
		for (p = startReplace + 1; p < (*input)->length; p++) {
			if ((*input)->chars[p] == startCharDots) level--;
			if ((*input)->chars[p] == endCharDots) level++;
			if (level == 1) break;
		}
		if (p == (*input)->length)
			return 0;
		else {
			int idx = getStringBuffer((*input)->length);
			widechar *chars = stringBufferPool->buffers[idx];
			memcpy(chars, (*input)->chars, (*input)->length * sizeof(widechar));
			chars[startReplace] = replaceStart;
			chars[p] = replaceEnd;
			*input = &(InString){
				.chars = chars, .length = (*input)->length, .bufferIndex = idx
			};
		}
	} else {
		if (transOpcode == CTO_Context) {
			startCharDots = groupingRule->charsdots[2];
			endCharDots = groupingRule->charsdots[3];
			replaceStart = replaceRule->charsdots[2];
			replaceEnd = replaceRule->charsdots[3];
		}
		output->chars[output->length] = replaceEnd;
		for (p = output->length - 1; p >= 0; p--) {
			if (output->chars[p] == endCharDots) level--;
			if (output->chars[p] == startCharDots) level++;
			if (level == 1) break;
		}
		if (p < 0) return 0;
		output->chars[p] = replaceStart;
		output->length++;
	}
	return 1;
}
