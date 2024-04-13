putCompChar(widechar character, const TranslationTableHeader *table, int pos,
		const InString *input, OutString *output, int *posMapping, int *cursorPosition,
		int *cursorStatus) {
	/* Insert the dots equivalent of a character into the output buffer */
	widechar d;
	TranslationTableOffset offset = (findCharOrDots(character, 0, table))->definitionRule;
	if (offset) {
		const TranslationTableRule *rule =
				(TranslationTableRule *)&table->ruleArea[offset];
		if (rule->dotslen)
			return for_updatePositions(&rule->charsdots[1], 1, rule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
		d = _lou_getDotsForChar(character);
		return for_updatePositions(&d, 1, 1, 0, pos, input, output, posMapping,
				cursorPosition, cursorStatus);
	}
	return undefinedCharacter(character, table, pos, input, output, posMapping,
			cursorPosition, cursorStatus);
}
