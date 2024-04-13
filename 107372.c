resolveEmphasisAllCapsSymbols(
		EmphasisInfo *buffer, formtype *typebuf, const InString *input) {
	/* Marks every caps letter with capsEmphClass symbol.
	 * Used in the special case where capsnocont has been defined and capsword has not
	 * been defined. */

	int inEmphasis = 0, i;

	for (i = 0; i < input->length; i++) {
		if (buffer[i].end & capsEmphClass) {
			inEmphasis = 0;
			buffer[i].end &= ~capsEmphClass;
		} else {
			if (buffer[i].begin & capsEmphClass) {
				buffer[i].begin &= ~capsEmphClass;
				inEmphasis = 1;
			}
			if (inEmphasis) {
				if (typebuf[i] & CAPSEMPH)
					/* Only mark if actually a capital letter (don't mark spaces or
					 * punctuation). */
					buffer[i].symbol |= capsEmphClass;
			} /* In emphasis */
		}	 /* Not caps end */
	}
}
