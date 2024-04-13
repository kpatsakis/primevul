doCompTrans(int start, int end, const TranslationTableHeader *table, int *pos,
		const InString *input, OutString *output, int *posMapping,
		EmphasisInfo *emphasisBuffer, const TranslationTableRule **transRule,
		int *cursorPosition, int *cursorStatus) {
	const TranslationTableRule *indicRule;
	int k;
	int haveEndsegment = 0;
	if (*cursorStatus != 2 && brailleIndicatorDefined(table->begComp, table, &indicRule))
		if (!for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, *pos,
					input, output, posMapping, cursorPosition, cursorStatus))
			return 0;
	for (k = start; k < end; k++) {
		TranslationTableOffset compdots = 0;
		/* HACK: computer braille is one-to-one so it
		 * can't have any emphasis indicators.
		 * A better solution is to treat computer braille as its own mode. */
		emphasisBuffer[k] = (EmphasisInfo){ 0 };
		if (input->chars[k] == ENDSEGMENT) {
			haveEndsegment = 1;
			continue;
		}
		*pos = k;
		if (input->chars[k] < 256) compdots = table->compdotsPattern[input->chars[k]];
		if (compdots != 0) {
			*transRule = (TranslationTableRule *)&table->ruleArea[compdots];
			if (!for_updatePositions(&(*transRule)->charsdots[(*transRule)->charslen],
						(*transRule)->charslen, (*transRule)->dotslen, 0, *pos, input,
						output, posMapping, cursorPosition, cursorStatus))
				return 0;
		} else if (!putCompChar(input->chars[k], table, *pos, input, output, posMapping,
						   cursorPosition, cursorStatus))
			return 0;
	}
	if (*cursorStatus != 2 && brailleIndicatorDefined(table->endComp, table, &indicRule))
		if (!for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, *pos,
					input, output, posMapping, cursorPosition, cursorStatus))
			return 0;
	*pos = end;
	if (haveEndsegment) {
		widechar endSegment = ENDSEGMENT;
		if (!for_updatePositions(&endSegment, 0, 1, 0, *pos, input, output, posMapping,
					cursorPosition, cursorStatus))
			return 0;
	}
	return 1;
}
