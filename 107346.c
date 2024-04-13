insertBrailleIndicators(int finish, const TranslationTableHeader *table, int pos,
		const InString *input, OutString *output, int *posMapping, formtype *typebuf,
		int haveEmphasis, int transOpcode, int prevTransOpcode, int *cursorPosition,
		int *cursorStatus, TranslationTableCharacterAttributes beforeAttributes,
		int *prevType, int *curType, int *prevTypeform, int prevPos) {
	/* Insert braille indicators such as letter, number, etc. */
	typedef enum {
		checkNothing,
		checkBeginTypeform,
		checkEndTypeform,
		checkNumber,
		checkLetter
	} checkThis;
	checkThis checkWhat = checkNothing;
	int ok = 0;
	int k;
	{
		if (pos == prevPos && !finish) return 1;
		if (pos != prevPos) {
			if (haveEmphasis && (typebuf[pos] & EMPHASIS) != *prevTypeform) {
				*prevType = *prevTypeform & EMPHASIS;
				*curType = typebuf[pos] & EMPHASIS;
				checkWhat = checkEndTypeform;
			} else if (!finish)
				checkWhat = checkNothing;
			else
				checkWhat = checkNumber;
		}
		if (finish == 1) checkWhat = checkNumber;
	}
	do {
		const TranslationTableRule *indicRule;
		ok = 0;
		switch (checkWhat) {
		case checkNothing:
			ok = 0;
			break;
		case checkBeginTypeform:
			if (haveEmphasis) {
				ok = 0;
				*curType = 0;
			}
			if (*curType == plain_text) {
				if (!finish)
					checkWhat = checkNothing;
				else
					checkWhat = checkNumber;
			}
			break;
		case checkEndTypeform:
			if (haveEmphasis) {
				ok = 0;
				*prevType = plain_text;
			}
			if (*prevType == plain_text) {
				checkWhat = checkBeginTypeform;
				*prevTypeform = typebuf[pos] & EMPHASIS;
			}
			break;
		case checkNumber:
			if (brailleIndicatorDefined(table->numberSign, table, &indicRule) &&
					checkAttr_safe(input, pos, CTC_Digit, 0, table) &&
					(prevTransOpcode == CTO_ExactDots ||
							!(beforeAttributes & CTC_Digit)) &&
					prevTransOpcode != CTO_MidNum) {
				ok = !table->usesNumericMode;
				checkWhat = checkNothing;
			} else
				checkWhat = checkLetter;
			break;
		case checkLetter:
			if (!brailleIndicatorDefined(table->letterSign, table, &indicRule)) {
				ok = 0;
				checkWhat = checkNothing;
				break;
			}
			if (transOpcode == CTO_Contraction) {
				ok = 1;
				checkWhat = checkNothing;
				break;
			}
			if ((checkAttr_safe(input, pos, CTC_Letter, 0, table) &&
						!(beforeAttributes & CTC_Letter)) &&
					(!checkAttr_safe(input, pos + 1, CTC_Letter, 0, table) ||
							(beforeAttributes & CTC_Digit))) {
				ok = 1;
				if (pos > 0)
					for (k = 0; k < table->noLetsignBeforeCount; k++)
						if (input->chars[pos - 1] == table->noLetsignBefore[k]) {
							ok = 0;
							break;
						}
				for (k = 0; k < table->noLetsignCount; k++)
					if (input->chars[pos] == table->noLetsign[k]) {
						ok = 0;
						break;
					}
				if (pos + 1 < input->length)
					for (k = 0; k < table->noLetsignAfterCount; k++)
						if (input->chars[pos + 1] == table->noLetsignAfter[k]) {
							ok = 0;
							break;
						}
			}
			checkWhat = checkNothing;
			break;

		default:
			ok = 0;
			checkWhat = checkNothing;
			break;
		}
		if (ok && indicRule != NULL) {
			if (!for_updatePositions(&indicRule->charsdots[0], 0, indicRule->dotslen, 0,
						pos, input, output, posMapping, cursorPosition, cursorStatus))
				return 0;
		}
	} while (checkWhat != checkNothing);
	return 1;
}
