_lou_translateWithTracing(const char *tableList, const widechar *inbufx, int *inlen,
		widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int *outputPos,
		int *inputPos, int *cursorPos, int mode, const TranslationTableRule **rules,
		int *rulesLen) {
	const TranslationTableHeader *table;
	const InString *input;
	OutString output;
	int *posMapping;
	int *posMapping1;
	int *posMapping2;
	int *posMapping3;
	formtype *typebuf;
	unsigned char *srcSpacing;
	unsigned char *destSpacing;
	unsigned int *wordBuffer;
	EmphasisInfo *emphasisBuffer;
	int cursorPosition;
	int cursorStatus;
	int haveEmphasis;
	int compbrlStart = -1;
	int compbrlEnd = -1;
	int k;
	int goodTrans = 1;
	int posIncremented;
	if (tableList == NULL || inbufx == NULL || inlen == NULL || outbuf == NULL ||
			outlen == NULL)
		return 0;
	_lou_logMessage(
			LOG_ALL, "Performing translation: tableList=%s, inlen=%d", tableList, *inlen);
	_lou_logWidecharBuf(LOG_ALL, "Inbuf=", inbufx, *inlen);

	if (mode & pass1Only) {
		_lou_logMessage(LOG_WARN, "warning: pass1Only mode is no longer supported.");
	}

	table = lou_getTable(tableList);
	if (table == NULL || *inlen < 0 || *outlen < 0) return 0;
	k = 0;
	while (k < *inlen && inbufx[k]) k++;
	input = &(InString){.chars = inbufx, .length = k, .bufferIndex = -1 };
	haveEmphasis = 0;
	if (!(typebuf = _lou_allocMem(alloc_typebuf, 0, input->length, *outlen))) return 0;
	if (typeform != NULL) {
		for (k = 0; k < input->length; k++) {
			typebuf[k] = typeform[k];
			if (typebuf[k] & EMPHASIS) haveEmphasis = 1;
		}
	} else
		memset(typebuf, 0, input->length * sizeof(formtype));

	if ((wordBuffer = _lou_allocMem(alloc_wordBuffer, 0, input->length, *outlen)))
		memset(wordBuffer, 0, (input->length + 4) * sizeof(unsigned int));
	else
		return 0;
	if ((emphasisBuffer = _lou_allocMem(alloc_emphasisBuffer, 0, input->length, *outlen)))
		memset(emphasisBuffer, 0, (input->length + 4) * sizeof(EmphasisInfo));
	else
		return 0;

	if (!(spacing == NULL || *spacing == 'X'))
		srcSpacing = (unsigned char *)spacing;
	else
		srcSpacing = NULL;
	if (outputPos != NULL)
		for (k = 0; k < input->length; k++) outputPos[k] = -1;
	if (cursorPos != NULL && *cursorPos >= 0) {
		cursorStatus = 0;
		cursorPosition = *cursorPos;
		if ((mode & (compbrlAtCursor | compbrlLeftCursor))) {
			compbrlStart = cursorPosition;
			if (checkAttr(input->chars[compbrlStart], CTC_Space, 0, table))
				compbrlEnd = compbrlStart + 1;
			else {
				while (compbrlStart >= 0 &&
						!checkAttr(input->chars[compbrlStart], CTC_Space, 0, table))
					compbrlStart--;
				compbrlStart++;
				compbrlEnd = cursorPosition;
				if (!(mode & compbrlLeftCursor))
					while (compbrlEnd < input->length &&
							!checkAttr(input->chars[compbrlEnd], CTC_Space, 0, table))
						compbrlEnd++;
			}
		}
	} else {
		cursorPosition = -1;
		cursorStatus = 1; /* so it won't check cursor position */
	}
	if (!(posMapping1 = _lou_allocMem(alloc_posMapping1, 0, input->length, *outlen)))
		return 0;
	if (table->numPasses > 1 || table->corrections) {
		if (!(posMapping2 = _lou_allocMem(alloc_posMapping2, 0, input->length, *outlen)))
			return 0;
		if (!(posMapping3 = _lou_allocMem(alloc_posMapping3, 0, input->length, *outlen)))
			return 0;
	}
	if (srcSpacing != NULL) {
		if (!(destSpacing = _lou_allocMem(alloc_destSpacing, 0, input->length, *outlen)))
			goodTrans = 0;
		else
			memset(destSpacing, '*', *outlen);
	} else
		destSpacing = NULL;
	appliedRulesCount = 0;
	if (rules != NULL && rulesLen != NULL) {
		appliedRules = rules;
		maxAppliedRules = *rulesLen;
	} else {
		appliedRules = NULL;
		maxAppliedRules = 0;
	}
	{
		int idx;
		if (!stringBufferPool) initStringBufferPool();
		for (idx = 0; idx < stringBufferPool->size; idx++) releaseStringBuffer(idx);
		idx = getStringBuffer(*outlen);
		output = (OutString){.chars = stringBufferPool->buffers[idx],
			.maxlength = *outlen,
			.length = 0,
			.bufferIndex = idx };
	}
	posMapping = posMapping1;

	int currentPass = table->corrections ? 0 : 1;
	int *passPosMapping = posMapping;
	while (1) {
		int realInlen;
		switch (currentPass) {
		case 0:
			goodTrans = makeCorrections(table, input, &output, passPosMapping, typebuf,
					&realInlen, &posIncremented, &cursorPosition, &cursorStatus);
			break;
		case 1: {
			goodTrans = translateString(table, mode, currentPass, input, &output,
					passPosMapping, typebuf, srcSpacing, destSpacing, wordBuffer,
					emphasisBuffer, haveEmphasis, &realInlen, &posIncremented,
					&cursorPosition, &cursorStatus, compbrlStart, compbrlEnd);
			break;
		}
		default:
			goodTrans = translatePass(table, currentPass, input, &output, passPosMapping,
					&realInlen, &posIncremented, &cursorPosition, &cursorStatus);
			break;
		}
		passPosMapping[output.length] = realInlen;
		if (passPosMapping == posMapping) {
			passPosMapping = posMapping2;
		} else {
			int *prevPosMapping = posMapping3;
			memcpy((int *)prevPosMapping, posMapping, (*outlen + 1) * sizeof(int));
			for (k = 0; k <= output.length; k++)
				if (passPosMapping[k] < 0)
					posMapping[k] = prevPosMapping[0];
				else
					posMapping[k] = prevPosMapping[passPosMapping[k]];
		}
		currentPass++;
		if (currentPass <= table->numPasses && goodTrans) {
			int idx;
			releaseStringBuffer(input->bufferIndex);
			input = &(InString){.chars = output.chars,
				.length = output.length,
				.bufferIndex = output.bufferIndex };
			idx = getStringBuffer(*outlen);
			output = (OutString){.chars = stringBufferPool->buffers[idx],
				.maxlength = *outlen,
				.length = 0,
				.bufferIndex = idx };
			continue;
		}
		break;
	}
	if (goodTrans) {
		for (k = 0; k < output.length; k++) {
			if (typeform != NULL) {
				if ((output.chars[k] & (B7 | B8)))
					typeform[k] = '8';
				else
					typeform[k] = '0';
			}
			if ((mode & dotsIO)) {
				if ((mode & ucBrl))
					outbuf[k] = ((output.chars[k] & 0xff) | 0x2800);
				else
					outbuf[k] = output.chars[k];
			} else
				outbuf[k] = _lou_getCharFromDots(output.chars[k]);
		}
		*inlen = posMapping[output.length];
		*outlen = output.length;
		if (inputPos != NULL) {
			for (k = 0; k < *outlen; k++)
				if (posMapping[k] < 0)
					inputPos[k] = 0;
				else if (posMapping[k] > *inlen - 1)
					inputPos[k] = *inlen - 1;
				else
					inputPos[k] = posMapping[k];
		}
		if (outputPos != NULL) {
			int inpos = -1;
			int outpos = -1;
			for (k = 0; k < *outlen; k++)
				if (posMapping[k] > inpos) {
					while (inpos < posMapping[k]) {
						if (inpos >= 0 && inpos < *inlen)
							outputPos[inpos] = outpos < 0 ? 0 : outpos;
						inpos++;
					}
					outpos = k;
				}
			if (inpos < 0) inpos = 0;
			while (inpos < *inlen) outputPos[inpos++] = outpos;
		}
	}
	if (destSpacing != NULL) {
		memcpy(srcSpacing, destSpacing, input->length);
		srcSpacing[input->length] = 0;
	}
	if (cursorPos != NULL && *cursorPos != -1) {
		if (outputPos != NULL)
			*cursorPos = outputPos[*cursorPos];
		else
			*cursorPos = cursorPosition;
	}
	if (rulesLen != NULL) *rulesLen = appliedRulesCount;
	_lou_logMessage(LOG_ALL, "Translation complete: outlen=%d", *outlen);
	_lou_logWidecharBuf(LOG_ALL, "Outbuf=", (const widechar *)outbuf, *outlen);

	return goodTrans;
}
