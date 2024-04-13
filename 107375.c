resolveEmphasisSingleSymbols(
		EmphasisInfo *buffer, const EmphasisClass class, const InString *input) {
	int i;

	for (i = 0; i < input->length; i++) {
		if (buffer[i].begin & class)
			if (buffer[i + 1].end & class) {
				buffer[i].begin &= ~class;
				buffer[i + 1].end &= ~class;
				buffer[i].symbol |= class;
			}
	}
}
