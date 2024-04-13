validMatch(const TranslationTableHeader *table, int pos, const InString *input,
		formtype *typebuf, const TranslationTableRule *transRule, int transCharslen) {
	/* Analyze the typeform parameter and also check for capitalization */
	TranslationTableCharacter *inputChar;
	TranslationTableCharacter *ruleChar;
	TranslationTableCharacterAttributes prevAttr = 0;
	int k;
	int kk = 0;
	if (!transCharslen) return 0;
	for (k = pos; k < pos + transCharslen; k++) {
		if (input->chars[k] == ENDSEGMENT) {
			if (k == pos && transCharslen == 1)
				return 1;
			else
				return 0;
		}
		inputChar = findCharOrDots(input->chars[k], 0, table);
		if (k == pos) prevAttr = inputChar->attributes;
		ruleChar = findCharOrDots(transRule->charsdots[kk++], 0, table);
		if ((inputChar->lowercase != ruleChar->lowercase)) return 0;
		if (typebuf != NULL && (typebuf[pos] & CAPSEMPH) == 0 &&
				(typebuf[k] | typebuf[pos]) != typebuf[pos])
			return 0;
		if (inputChar->attributes != CTC_Letter) {
			if (k != (pos + 1) && (prevAttr & CTC_Letter) &&
					(inputChar->attributes & CTC_Letter) &&
					((inputChar->attributes &
							 (CTC_LowerCase | CTC_UpperCase | CTC_Letter)) !=
							(prevAttr & (CTC_LowerCase | CTC_UpperCase | CTC_Letter))))
				return 0;
		}
		prevAttr = inputChar->attributes;
	}
	return 1;
}
