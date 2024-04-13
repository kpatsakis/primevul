translateString(const TranslationTableHeader *table, int mode, int currentPass,
		const InString *input, OutString *output, int *posMapping, formtype *typebuf,
		unsigned char *srcSpacing, unsigned char *destSpacing, unsigned int *wordBuffer,
		EmphasisInfo *emphasisBuffer, int haveEmphasis, int *realInlen,
		int *posIncremented, int *cursorPosition, int *cursorStatus, int compbrlStart,
		int compbrlEnd) {
	int pos;
	int transOpcode;
	int prevTransOpcode;
	const TranslationTableRule *transRule;
	int transCharslen;
	int passCharDots;
	const widechar *passInstructions;
	int passIC; /* Instruction counter */
	PassRuleMatch patternMatch;
	TranslationTableRule *groupingRule;
	widechar groupingOp;
	int numericMode;
	int dontContract;
	int destword;
	int srcword;
	int pre_src;
	TranslationTableCharacter *curCharDef;
	const widechar *repwordStart;
	int repwordLength;
	int curType;
	int prevType;
	int prevTypeform;
	int prevPos;
	const InString *origInput = input;
	/* Main translation routine */
	int k;
	translation_direction = 1;
	markSyllables(table, input, typebuf, &transOpcode, &transRule, &transCharslen);
	numericMode = 0;
	srcword = 0;
	destword = 0; /* last word translated */
	dontContract = 0;
	prevTransOpcode = CTO_None;
	prevType = curType = prevTypeform = plain_text;
	prevPos = -1;
	pos = output->length = 0;
	*posIncremented = 1;
	pre_src = 0;
	_lou_resetPassVariables();
	if (typebuf && table->emphRules[capsRule][letterOffset])
		for (k = 0; k < input->length; k++)
			if (checkAttr(input->chars[k], CTC_UpperCase, 0, table))
				typebuf[k] |= CAPSEMPH;

	markEmphases(table, input, typebuf, wordBuffer, emphasisBuffer, haveEmphasis);

	while (pos < input->length) { /* the main translation loop */
		if ((pos >= compbrlStart) && (pos < compbrlEnd)) {
			int cs = 2;  // cursor status for this call
			if (!doCompTrans(pos, compbrlEnd, table, &pos, input, output, posMapping,
						emphasisBuffer, &transRule, cursorPosition, &cs))
				goto failure;
			continue;
		}
		TranslationTableCharacterAttributes beforeAttributes;
		setBefore(table, pos, input, &beforeAttributes);
		if (!insertBrailleIndicators(0, table, pos, input, output, posMapping, typebuf,
					haveEmphasis, transOpcode, prevTransOpcode, cursorPosition,
					cursorStatus, beforeAttributes, &prevType, &curType, &prevTypeform,
					prevPos))
			goto failure;
		if (pos >= input->length) break;

		if (!dontContract) dontContract = typebuf[pos] & no_contract;
		if (typebuf[pos] & no_translate) {
			widechar c = _lou_getDotsForChar(input->chars[pos]);
			if (input->chars[pos] < 32 || input->chars[pos] > 126) goto failure;
			if (!for_updatePositions(&c, 1, 1, 0, pos, input, output, posMapping,
						cursorPosition, cursorStatus))
				goto failure;
			pos++;
			/* because we don't call insertEmphasis */
			pre_src = pos;
			continue;
		}
		for_selectRule(table, pos, *output, mode, input, typebuf, emphasisBuffer,
				&transOpcode, prevTransOpcode, &transRule, &transCharslen, &passCharDots,
				&passInstructions, &passIC, &patternMatch, *posIncremented,
				*cursorPosition, &repwordStart, &repwordLength, dontContract,
				compbrlStart, compbrlEnd, beforeAttributes, &curCharDef, &groupingRule,
				&groupingOp);

		if (transOpcode != CTO_Context)
			if (appliedRules != NULL && appliedRulesCount < maxAppliedRules)
				appliedRules[appliedRulesCount++] = transRule;
		*posIncremented = 1;
		prevPos = pos;
		switch (transOpcode) /* Rules that pre-empt context and swap */
		{
		case CTO_CompBrl:
		case CTO_Literal:
			if (!doCompbrl(table, &pos, input, output, posMapping, emphasisBuffer,
						&transRule, cursorPosition, cursorStatus, destword, srcword))
				goto failure;
			continue;
		default:
			break;
		}
		if (!insertBrailleIndicators(1, table, pos, input, output, posMapping, typebuf,
					haveEmphasis, transOpcode, prevTransOpcode, cursorPosition,
					cursorStatus, beforeAttributes, &prevType, &curType, &prevTypeform,
					prevPos))
			goto failure;

		insertEmphases(table, pos, input, output, posMapping, emphasisBuffer,
				haveEmphasis, transOpcode, cursorPosition, cursorStatus, &pre_src);
		if (table->usesNumericMode)
			checkNumericMode(table, pos, input, output, posMapping, cursorPosition,
					cursorStatus, &dontContract, &numericMode);

		if (transOpcode == CTO_Context ||
				findForPassRule(table, pos, currentPass, input, &transOpcode, &transRule,
						&transCharslen, &passCharDots, &passInstructions, &passIC,
						&patternMatch, &groupingRule, &groupingOp))
			switch (transOpcode) {
			case CTO_Context: {
				const InString *inputBefore = input;
				int posBefore = pos;
				if (appliedRules != NULL && appliedRulesCount < maxAppliedRules)
					appliedRules[appliedRulesCount++] = transRule;
				if (!passDoAction(table, &input, output, posMapping, transOpcode,
							&transRule, passCharDots, passInstructions, passIC, &pos,
							patternMatch, cursorPosition, cursorStatus, groupingRule,
							groupingOp))
					goto failure;
				if (input->bufferIndex != inputBefore->bufferIndex &&
						inputBefore->bufferIndex != origInput->bufferIndex)
					releaseStringBuffer(inputBefore->bufferIndex);
				if (pos == posBefore) *posIncremented = 0;
				continue;
			}
			default:
				break;
			}

		/* Processing before replacement */

		/* check if leaving no contraction (grade 1) mode */
		if (checkAttr(input->chars[pos], CTC_SeqDelimiter | CTC_Space, 0, table))
			dontContract = 0;

		switch (transOpcode) {
		case CTO_EndNum:
			if (table->letterSign && checkAttr(input->chars[pos], CTC_Letter, 0, table))
				output->length--;
			break;
		case CTO_Repeated:
		case CTO_Space:
			dontContract = 0;
			break;
		case CTO_LargeSign:
			if (prevTransOpcode == CTO_LargeSign) {
				int hasEndSegment = 0;
				while (output->length > 0 && checkAttr(output->chars[output->length - 1],
													 CTC_Space, 1, table)) {
					if (output->chars[output->length - 1] == ENDSEGMENT) {
						hasEndSegment = 1;
					}
					output->length--;
				}
				if (hasEndSegment != 0) {
					output->chars[output->length] = 0xffff;
					output->length++;
				}
			}
			break;
		case CTO_DecPoint:
			if (!table->usesNumericMode && table->numberSign) {
				TranslationTableRule *numRule =
						(TranslationTableRule *)&table->ruleArea[table->numberSign];
				if (!for_updatePositions(&numRule->charsdots[numRule->charslen],
							numRule->charslen, numRule->dotslen, 0, pos, input, output,
							posMapping, cursorPosition, cursorStatus))
					goto failure;
			}
			transOpcode = CTO_MidNum;
			break;
		case CTO_NoCont:
			if (!dontContract)
				doNocont(table, &pos, output, mode, input, destword, srcword,
						&dontContract);
			continue;
		default:
			break;
		} /* end of action */

		/* replacement processing */
		switch (transOpcode) {
		case CTO_Replace:
			pos += transCharslen;
			if (!putCharacters(&transRule->charsdots[transCharslen], transRule->dotslen,
						table, pos, input, output, posMapping, cursorPosition,
						cursorStatus))
				goto failure;
			break;
		case CTO_None:
			if (!undefinedCharacter(input->chars[pos], table, pos, input, output,
						posMapping, cursorPosition, cursorStatus))
				goto failure;
			pos++;
			break;
		case CTO_UpperCase:
			/* Only needs special handling if not within compbrl and
			 * the table defines a capital sign. */
			if (!(mode & (compbrlAtCursor | compbrlLeftCursor)) &&
					(transRule->dotslen == 1 &&
							table->emphRules[capsRule][letterOffset])) {
				if (!putCharacter(curCharDef->lowercase, table, pos, input, output,
							posMapping, cursorPosition, cursorStatus))
					goto failure;
				pos++;
				break;
			}

		default:
			if (transRule->dotslen) {
				if (!for_updatePositions(&transRule->charsdots[transCharslen],
							transCharslen, transRule->dotslen, 0, pos, input, output,
							posMapping, cursorPosition, cursorStatus))
					goto failure;
				pos += transCharslen;
			} else {
				for (k = 0; k < transCharslen; k++) {
					if (!putCharacter(input->chars[pos], table, pos, input, output,
								posMapping, cursorPosition, cursorStatus))
						goto failure;
					pos++;
				}
			}
			break;
		}

		/* processing after replacement */
		switch (transOpcode) {
		case CTO_Repeated: {
			/* Skip repeated characters. */
			int srclim = input->length - transCharslen;
			if (mode & (compbrlAtCursor | compbrlLeftCursor) && compbrlStart < srclim)
				/* Don't skip characters from compbrlStart onwards. */
				srclim = compbrlStart - 1;
			while ((pos <= srclim) &&
					compareChars(&transRule->charsdots[0], &input->chars[pos],
							transCharslen, 0, table)) {
				/* Map skipped input positions to the previous output position. */
				/* if (posMapping.outputPositions != NULL) { */
				/* 	int tcc; */
				/* 	for (tcc = 0; tcc < transCharslen; tcc++) */
				/* 		posMapping.outputPositions[posMapping.prev[pos + tcc]] = */
				/* 				output->length - 1; */
				/* } */
				if (!*cursorStatus && pos <= *cursorPosition &&
						*cursorPosition < pos + transCharslen) {
					*cursorStatus = 1;
					*cursorPosition = output->length - 1;
				}
				pos += transCharslen;
			}
			break;
		}
		case CTO_RepWord: {
			/* Skip repeated characters. */
			int srclim = input->length - transCharslen;
			if (mode & (compbrlAtCursor | compbrlLeftCursor) && compbrlStart < srclim)
				/* Don't skip characters from compbrlStart onwards. */
				srclim = compbrlStart - 1;
			while ((pos <= srclim) && compareChars(repwordStart, &input->chars[pos],
											  repwordLength, 0, table)) {
				/* Map skipped input positions to the previous output position. */
				/* if (posMapping.outputPositions != NULL) { */
				/* 	int tcc; */
				/* 	for (tcc = 0; tcc < transCharslen; tcc++) */
				/* 		posMapping.outputPositions[posMapping.prev[pos + tcc]] = */
				/* 				output->length - 1; */
				/* } */
				if (!*cursorStatus && pos <= *cursorPosition &&
						*cursorPosition < pos + transCharslen) {
					*cursorStatus = 1;
					*cursorPosition = output->length - 1;
				}
				pos += repwordLength + transCharslen;
			}
			pos -= transCharslen;
			break;
		}
		case CTO_JoinNum:
		case CTO_JoinableWord:
			while (pos < input->length &&
					checkAttr(input->chars[pos], CTC_Space, 0, table) &&
					input->chars[pos] != ENDSEGMENT)
				pos++;
			break;
		default:
			break;
		}
		if (((pos > 0) && checkAttr(input->chars[pos - 1], CTC_Space, 0, table) &&
					(transOpcode != CTO_JoinableWord))) {
			srcword = pos;
			destword = output->length;
		}
		if (srcSpacing != NULL && srcSpacing[pos] >= '0' && srcSpacing[pos] <= '9')
			destSpacing[output->length] = srcSpacing[pos];
		if ((transOpcode >= CTO_Always && transOpcode <= CTO_None) ||
				(transOpcode >= CTO_Digit && transOpcode <= CTO_LitDigit))
			prevTransOpcode = transOpcode;
	}

	transOpcode = CTO_Space;
	insertEmphases(table, pos, input, output, posMapping, emphasisBuffer, haveEmphasis,
			transOpcode, cursorPosition, cursorStatus, &pre_src);

failure:
	if (destword != 0 && pos < input->length &&
			!checkAttr(input->chars[pos], CTC_Space, 0, table)) {
		pos = srcword;
		output->length = destword;
	}
	if (pos < input->length) {
		while (checkAttr(input->chars[pos], CTC_Space, 0, table))
			if (++pos == input->length) break;
	}
	*realInlen = pos;
	if (input->bufferIndex != origInput->bufferIndex)
		releaseStringBuffer(input->bufferIndex);
	return 1;
} /* first pass translation completed */
