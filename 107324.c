beginCount(const EmphasisInfo *buffer, const int at, const EmphasisClass class,
		const TranslationTableHeader *table, const InString *input) {
	if (buffer[at].begin & class) {
		int i, cnt = 1;
		for (i = at + 1; i < input->length; i++)
			if (buffer[i].end & class)
				break;
			else
				cnt++;
		return cnt;
	} else if (buffer[at].word & class) {
		int i, cnt = 1;
		for (i = at + 1; i < input->length; i++)
			if (buffer[i].end & class) break;
			else if (checkAttr(input->chars[i], CTC_SeqDelimiter | CTC_Space, 0, table))
				break;
			else
				cnt++;
		return cnt;
	}
	return 0;
}
