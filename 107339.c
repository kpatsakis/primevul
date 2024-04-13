for_selectRule(const TranslationTableHeader *table, int pos, OutString output, int mode,
		const InString *input, formtype *typebuf, EmphasisInfo *emphasisBuffer,
		int *transOpcode, int prevTransOpcode, const TranslationTableRule **transRule,
		int *transCharslen, int *passCharDots, widechar const **passInstructions,
		int *passIC, PassRuleMatch *patternMatch, int posIncremented, int cursorPosition,
		const widechar **repwordStart, int *repwordLength, int dontContract,
		int compbrlStart, int compbrlEnd,
		TranslationTableCharacterAttributes beforeAttributes,
		TranslationTableCharacter **curCharDef, TranslationTableRule **groupingRule,
		widechar *groupingOp) {
	/* check for valid Translations. Return value is in transRule. */
	static TranslationTableRule pseudoRule = { 0 };
	int length = ((pos < compbrlStart) ? compbrlStart : input->length) - pos;
	int tryThis;
	const TranslationTableCharacter *character2;
	int k;
	TranslationTableOffset ruleOffset = 0;
	*curCharDef = findCharOrDots(input->chars[pos], 0, table);
	for (tryThis = 0; tryThis < 3; tryThis++) {
		unsigned long int makeHash = 0;
		switch (tryThis) {
		case 0:
			if (!(length >= 2)) break;
			/* Hash function optimized for forward translation */
			makeHash = (unsigned long int)(*curCharDef)->lowercase << 8;
			character2 = findCharOrDots(input->chars[pos + 1], 0, table);
			makeHash += (unsigned long int)character2->lowercase;
			makeHash %= HASHNUM;
			ruleOffset = table->forRules[makeHash];
			break;
		case 1:
			if (!(length >= 1)) break;
			length = 1;
			ruleOffset = (*curCharDef)->otherRules;
			break;
		case 2: /* No rule found */
			*transRule = &pseudoRule;
			*transOpcode = pseudoRule.opcode = CTO_None;
			*transCharslen = pseudoRule.charslen = 1;
			pseudoRule.charsdots[0] = input->chars[pos];
			pseudoRule.dotslen = 0;
			return;
			break;
		}
		while (ruleOffset) {
			*transRule = (TranslationTableRule *)&table->ruleArea[ruleOffset];
			*transOpcode = (*transRule)->opcode;
			*transCharslen = (*transRule)->charslen;
			if (tryThis == 1 ||
					((*transCharslen <= length) && validMatch(table, pos, input, typebuf,
														   *transRule, *transCharslen))) {
				TranslationTableCharacterAttributes afterAttributes;
				/* check before emphasis match */
				if ((*transRule)->before & CTC_EmpMatch) {
					if (emphasisBuffer[pos].begin || emphasisBuffer[pos].end ||
							emphasisBuffer[pos].word || emphasisBuffer[pos].symbol)
						break;
				}

				/* check after emphasis match */
				if ((*transRule)->after & CTC_EmpMatch) {
					if (emphasisBuffer[pos + *transCharslen].begin ||
							emphasisBuffer[pos + *transCharslen].end ||
							emphasisBuffer[pos + *transCharslen].word ||
							emphasisBuffer[pos + *transCharslen].symbol)
						break;
				}

				/* check this rule */
				setAfter(*transCharslen, table, pos, input, &afterAttributes);
				if ((!((*transRule)->after & ~CTC_EmpMatch) ||
							(beforeAttributes & (*transRule)->after)) &&
						(!((*transRule)->before & ~CTC_EmpMatch) ||
								(afterAttributes & (*transRule)->before)))
					switch (*transOpcode) { /* check validity of this Translation */
					case CTO_Space:
					case CTO_Letter:
					case CTO_UpperCase:
					case CTO_LowerCase:
					case CTO_Digit:
					case CTO_LitDigit:
					case CTO_Punctuation:
					case CTO_Math:
					case CTO_Sign:
					case CTO_Hyphen:
					case CTO_Replace:
					case CTO_CompBrl:
					case CTO_Literal:
						return;
					case CTO_Repeated:
						if ((mode & (compbrlAtCursor | compbrlLeftCursor)) &&
								pos >= compbrlStart && pos <= compbrlEnd)
							break;
						return;
					case CTO_RepWord:
						if (dontContract || (mode & noContractions)) break;
						if (isRepeatedWord(table, pos, input, *transCharslen,
									repwordStart, repwordLength))
							return;
						break;
					case CTO_NoCont:
						if (dontContract || (mode & noContractions)) break;
						return;
					case CTO_Syllable:
						*transOpcode = CTO_Always;
					case CTO_Always:
						if (checkEmphasisChange(0, pos, emphasisBuffer, *transRule))
							break;
						if (dontContract || (mode & noContractions)) break;
						return;
					case CTO_ExactDots:
						return;
					case CTO_NoCross:
						if (dontContract || (mode & noContractions)) break;
						if (syllableBreak(table, pos, input, *transCharslen)) break;
						return;
					case CTO_Context:
						if (!posIncremented ||
								!passDoTest(table, pos, input, *transOpcode, *transRule,
										passCharDots, passInstructions, passIC,
										patternMatch, groupingRule, groupingOp))
							break;
						return;
					case CTO_LargeSign:
						if (dontContract || (mode & noContractions)) break;
						if (!((beforeAttributes & (CTC_Space | CTC_Punctuation)) ||
									onlyLettersBehind(
											table, pos, input, beforeAttributes)) ||
								!((afterAttributes & CTC_Space) ||
										prevTransOpcode == CTO_LargeSign) ||
								(afterAttributes & CTC_Letter) ||
								!noCompbrlAhead(table, pos, mode, input, *transOpcode,
										*transCharslen, cursorPosition))
							*transOpcode = CTO_Always;
						return;
					case CTO_WholeWord:
						if (dontContract || (mode & noContractions)) break;
						if (checkEmphasisChange(0, pos, emphasisBuffer, *transRule))
							break;
					case CTO_Contraction:
						if (table->usesSequences) {
							if (inSequence(table, pos, input, *transRule)) return;
						} else {
							if ((beforeAttributes & (CTC_Space | CTC_Punctuation)) &&
									(afterAttributes & (CTC_Space | CTC_Punctuation)))
								return;
						}
						break;
					case CTO_PartWord:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes & CTC_Letter) ||
								(afterAttributes & CTC_Letter))
							return;
						break;
					case CTO_JoinNum:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes & (CTC_Space | CTC_Punctuation)) &&
								(afterAttributes & CTC_Space) &&
								(output.length + (*transRule)->dotslen <
										output.maxlength)) {
							int p = pos + *transCharslen + 1;
							while (p < input->length) {
								if (!checkAttr(input->chars[p], CTC_Space, 0, table)) {
									if (checkAttr(input->chars[p], CTC_Digit, 0, table))
										return;
									break;
								}
								p++;
							}
						}
						break;
					case CTO_LowWord:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes & CTC_Space) &&
								(afterAttributes & CTC_Space) &&
								(prevTransOpcode != CTO_JoinableWord))
							return;
						break;
					case CTO_JoinableWord:
						if (dontContract || (mode & noContractions)) break;
						if (beforeAttributes & (CTC_Space | CTC_Punctuation) &&
								onlyLettersAhead(table, pos, input, *transCharslen,
										afterAttributes) &&
								noCompbrlAhead(table, pos, mode, input, *transOpcode,
										*transCharslen, cursorPosition))
							return;
						break;
					case CTO_SuffixableWord:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes & (CTC_Space | CTC_Punctuation)) &&
								(afterAttributes &
										(CTC_Space | CTC_Letter | CTC_Punctuation)))
							return;
						break;
					case CTO_PrefixableWord:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes &
									(CTC_Space | CTC_Letter | CTC_Punctuation)) &&
								(afterAttributes & (CTC_Space | CTC_Punctuation)))
							return;
						break;
					case CTO_BegWord:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes & (CTC_Space | CTC_Punctuation)) &&
								(afterAttributes & CTC_Letter))
							return;
						break;
					case CTO_BegMidWord:
						if (dontContract || (mode & noContractions)) break;
						if ((beforeAttributes &
									(CTC_Letter | CTC_Space | CTC_Punctuation)) &&
								(afterAttributes & CTC_Letter))
							return;
						break;
					case CTO_MidWord:
						if (dontContract || (mode & noContractions)) break;
						if (beforeAttributes & CTC_Letter && afterAttributes & CTC_Letter)
							return;
						break;
					case CTO_MidEndWord:
						if (dontContract || (mode & noContractions)) break;
						if (beforeAttributes & CTC_Letter &&
								afterAttributes &
										(CTC_Letter | CTC_Space | CTC_Punctuation))
							return;
						break;
					case CTO_EndWord:
						if (dontContract || (mode & noContractions)) break;
						if (beforeAttributes & CTC_Letter &&
								afterAttributes & (CTC_Space | CTC_Punctuation))
							return;
						break;
					case CTO_BegNum:
						if (beforeAttributes & (CTC_Space | CTC_Punctuation) &&
								afterAttributes & CTC_Digit)
							return;
						break;
					case CTO_MidNum:
						if (prevTransOpcode != CTO_ExactDots &&
								beforeAttributes & CTC_Digit &&
								afterAttributes & CTC_Digit)
							return;
						break;
					case CTO_EndNum:
						if (beforeAttributes & CTC_Digit &&
								prevTransOpcode != CTO_ExactDots)
							return;
						break;
					case CTO_DecPoint:
						if (!(afterAttributes & CTC_Digit)) break;
						if (beforeAttributes & CTC_Digit) *transOpcode = CTO_MidNum;
						return;
					case CTO_PrePunc:
						if (!checkAttr(input->chars[pos], CTC_Punctuation, 0, table) ||
								(pos > 0 && checkAttr(input->chars[pos - 1], CTC_Letter,
													0, table)))
							break;
						for (k = pos + *transCharslen; k < input->length; k++) {
							if (checkAttr(input->chars[k], (CTC_Letter | CTC_Digit), 0,
										table))
								return;
							if (checkAttr(input->chars[k], CTC_Space, 0, table)) break;
						}
						break;
					case CTO_PostPunc:
						if (!checkAttr(input->chars[pos], CTC_Punctuation, 0, table) ||
								(pos < (input->length - 1) &&
										checkAttr(input->chars[pos + 1], CTC_Letter, 0,
												table)))
							break;
						for (k = pos; k >= 0; k--) {
							if (checkAttr(input->chars[k], (CTC_Letter | CTC_Digit), 0,
										table))
								return;
							if (checkAttr(input->chars[k], CTC_Space, 0, table)) break;
						}
						break;

					case CTO_Match: {
						widechar *patterns, *pattern;

						if (dontContract || (mode & noContractions)) break;
						if (checkEmphasisChange(0, pos, emphasisBuffer, *transRule))
							break;

						patterns = (widechar *)&table->ruleArea[(*transRule)->patterns];

						/* check before pattern */
						pattern = &patterns[1];
						if (!_lou_pattern_check(
									input->chars, pos - 1, -1, -1, pattern, table))
							break;

						/* check after pattern */
						pattern = &patterns[patterns[0]];
						if (!_lou_pattern_check(input->chars,
									pos + (*transRule)->charslen, input->length, 1,
									pattern, table))
							break;

						return;
					}

					default:
						break;
					}
			}
			/* Done with checking this rule */
			ruleOffset = (*transRule)->charsnext;
		}
	}
}
