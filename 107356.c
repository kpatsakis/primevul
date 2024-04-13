lou_translatePrehyphenated(const char *tableList, const widechar *inbufx, int *inlen,
		widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int *outputPos,
		int *inputPos, int *cursorPos, char *inputHyphens, char *outputHyphens,
		int mode) {
	int rv = 1;
	int *alloc_inputPos = NULL;
	if (inputHyphens != NULL) {
		if (outputHyphens == NULL) return 0;
		if (inputPos == NULL) {
			if ((alloc_inputPos = malloc(*outlen * sizeof(int))) == NULL)
				_lou_outOfMemory();
			inputPos = alloc_inputPos;
		}
	}
	if (lou_translate(tableList, inbufx, inlen, outbuf, outlen, typeform, spacing,
				outputPos, inputPos, cursorPos, mode)) {
		if (inputHyphens != NULL) {
			int inpos = 0;
			int outpos;
			for (outpos = 0; outpos < *outlen; outpos++) {
				int new_inpos = inputPos[outpos];
				if (new_inpos < inpos) {
					rv = 0;
					break;
				}
				if (new_inpos > inpos)
					outputHyphens[outpos] = inputHyphens[new_inpos];
				else
					outputHyphens[outpos] = '0';
				inpos = new_inpos;
			}
		}
	}
	if (alloc_inputPos != NULL) free(alloc_inputPos);
	return rv;
}
