for_updatePositions(const widechar *outChars, int inLength, int outLength, int shift,
		int pos, const InString *input, OutString *output, int *posMapping,
		int *cursorPosition, int *cursorStatus) {
	int k;
	if ((output->length + outLength) > output->maxlength ||
			(pos + inLength) > input->length)
		return 0;
	memcpy(&output->chars[output->length], outChars, outLength * CHARSIZE);
	if (!*cursorStatus) {
		if (*cursorPosition >= pos && *cursorPosition < (pos + inLength)) {
			*cursorPosition = output->length;
			*cursorStatus = 1;
		} else if (input->chars[*cursorPosition] == 0 &&
				*cursorPosition == (pos + inLength)) {
			*cursorPosition = output->length + outLength / 2 + 1;
			*cursorStatus = 1;
		}
	} else if (*cursorStatus == 2 && *cursorPosition == pos)
		*cursorPosition = output->length;
	for (k = 0; k < outLength; k++) posMapping[output->length + k] = pos + shift;
	output->length += outLength;
	return 1;
}
