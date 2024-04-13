hstoreFindKey(HStore *hs, int *lowbound, char *key, int keylen)
{
	HEntry	   *entries = ARRPTR(hs);
	int			stopLow = lowbound ? *lowbound : 0;
	int			stopHigh = HS_COUNT(hs);
	int			stopMiddle;
	char	   *base = STRPTR(hs);

	while (stopLow < stopHigh)
	{
		int			difference;

		stopMiddle = stopLow + (stopHigh - stopLow) / 2;

		if (HS_KEYLEN(entries, stopMiddle) == keylen)
			difference = memcmp(HS_KEY(entries, base, stopMiddle), key, keylen);
		else
			difference = (HS_KEYLEN(entries, stopMiddle) > keylen) ? 1 : -1;

		if (difference == 0)
		{
			if (lowbound)
				*lowbound = stopMiddle + 1;
			return stopMiddle;
		}
		else if (difference < 0)
			stopLow = stopMiddle + 1;
		else
			stopHigh = stopMiddle;
	}

	if (lowbound)
		*lowbound = stopLow;
	return -1;
}
