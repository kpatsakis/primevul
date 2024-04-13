undefinedCharacter(widechar c, const TranslationTableHeader *table, int pos,
		const InString *input, OutString *output, int *posMapping, int *cursorPosition,
		int *cursorStatus) {
	/* Display an undefined character in the output buffer */
	int k;
	char *display;
	widechar displayDots[20];
	if (table->undefined) {
		TranslationTableRule *rule =
				(TranslationTableRule *)&table->ruleArea[table->undefined];
		if (!for_updatePositions(&rule->charsdots[rule->charslen], rule->charslen,
					rule->dotslen, 0, pos, input, output, posMapping, cursorPosition,
					cursorStatus))
			return 0;
		return 1;
	}
	display = _lou_showString(&c, 1);
	for (k = 0; k < (int)strlen(display); k++)
		displayDots[k] = _lou_getDotsForChar(display[k]);
	if (!for_updatePositions(displayDots, 1, (int)strlen(display), 0, pos, input, output,
				posMapping, cursorPosition, cursorStatus))
		return 0;
	return 1;
}
