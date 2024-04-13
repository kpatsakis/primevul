checkAttr(const widechar c, const TranslationTableCharacterAttributes a, int m,
		const TranslationTableHeader *table) {
	static widechar prevc = 0;
	static TranslationTableCharacterAttributes preva = 0;
	if (c != prevc) {
		preva = (findCharOrDots(c, m, table))->attributes;
		prevc = c;
	}
	return ((preva & a) ? 1 : 0);
}
