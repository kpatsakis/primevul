checkEmphasisChange(const int skip, int pos, EmphasisInfo *emphasisBuffer,
		const TranslationTableRule *transRule) {
	int i;
	for (i = pos + (skip + 1); i < pos + transRule->charslen; i++)
		if (emphasisBuffer[i].begin || emphasisBuffer[i].end || emphasisBuffer[i].word ||
				emphasisBuffer[i].symbol)
			return 1;
	return 0;
}
