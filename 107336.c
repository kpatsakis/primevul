endCount(const EmphasisInfo *buffer, const int at, const EmphasisClass class) {
	int i, cnt = 1;
	if (!(buffer[at].end & class)) return 0;
	for (i = at - 1; i >= 0; i--)
		if (buffer[i].begin & class || buffer[i].word & class)
			break;
		else
			cnt++;
	return cnt;
}
