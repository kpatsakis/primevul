findCharOrDots(widechar c, int m, const TranslationTableHeader *table) {
	/* Look up character or dot pattern in the appropriate
	 * table. */
	static TranslationTableCharacter noChar = { 0, 0, 0, CTC_Space, 32, 32, 32 };
	static TranslationTableCharacter noDots = { 0, 0, 0, CTC_Space, B16, B16, B16 };
	TranslationTableCharacter *notFound;
	TranslationTableCharacter *character;
	TranslationTableOffset bucket;
	unsigned long int makeHash = (unsigned long int)c % HASHNUM;
	if (m == 0) {
		bucket = table->characters[makeHash];
		notFound = &noChar;
	} else {
		bucket = table->dots[makeHash];
		notFound = &noDots;
	}
	while (bucket) {
		character = (TranslationTableCharacter *)&table->ruleArea[bucket];
		if (character->realchar == c) return character;
		bucket = character->next;
	}
	notFound->realchar = notFound->uppercase = notFound->lowercase = c;
	return notFound;
}
