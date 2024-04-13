checkNumericMode(const TranslationTableHeader *table, int pos, const InString *input,
		OutString *output, int *posMapping, int *cursorPosition, int *cursorStatus,
		int *dontContract, int *numericMode) {
	int i;
	const TranslationTableRule *indicRule;
	if (!brailleIndicatorDefined(table->numberSign, table, &indicRule)) return;

	/* not in numeric mode */
	if (!*numericMode) {
		if (checkAttr(input->chars[pos], CTC_Digit | CTC_LitDigit, 0, table)) {
			*numericMode = 1;
			*dontContract = 1;
			for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0, pos,
					input, output, posMapping, cursorPosition, cursorStatus);
		} else if (checkAttr(input->chars[pos], CTC_NumericMode, 0, table)) {
			for (i = pos + 1; i < input->length; i++) {
				if (checkAttr(input->chars[i], CTC_Digit | CTC_LitDigit, 0, table)) {
					*numericMode = 1;
					for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen,
							0, pos, input, output, posMapping, cursorPosition,
							cursorStatus);
					break;
				} else if (!checkAttr(input->chars[i], CTC_NumericMode, 0, table))
					break;
			}
		}
	}

	/* in numeric mode */
	else {
		if (!checkAttr(input->chars[pos],
					CTC_Digit | CTC_LitDigit | CTC_NumericMode | CTC_MidEndNumericMode, 0,
					table)) {
			*numericMode = 0;
			if (brailleIndicatorDefined(table->noContractSign, table, &indicRule))
				if (checkAttr(input->chars[pos], CTC_NumericNoContract, 0, table))
					for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen,
							0, pos, input, output, posMapping, cursorPosition,
							cursorStatus);
		}
	}
}
