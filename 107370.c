removeGrouping(const InString **input, OutString *output, int passCharDots,
		int startReplace, TranslationTableRule *groupingRule, widechar groupingOp) {
	widechar startCharDots = groupingRule->charsdots[2 * passCharDots];
	widechar endCharDots = groupingRule->charsdots[2 * passCharDots + 1];
	int p;
	int level = 0;
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
			int len = 0;
			int k;
			for (k = 0; k < (*input)->length; k++) {
				if (k == p) continue;
				chars[len++] = (*input)->chars[k];
			}
			*input = &(InString){.chars = chars, .length = len, .bufferIndex = idx };
		}
	} else {
		for (p = output->length - 1; p >= 0; p--) {
			if (output->chars[p] == endCharDots) level--;
			if (output->chars[p] == startCharDots) level++;
			if (level == 1) break;
		}
		if (p < 0) return 0;
		p++;
		for (; p < output->length; p++) output->chars[p - 1] = output->chars[p];
		output->length--;
	}
	return 1;
}
