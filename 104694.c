BOOL CSoundFile::DestroySample(UINT nSample)
{
	if ((!nSample) || (nSample >= MAX_SAMPLES)) return FALSE;
	if (!Ins[nSample].pSample) return TRUE;
	MODINSTRUMENT *pins = &Ins[nSample];
	signed char *pSample = pins->pSample;
	pins->pSample = NULL;
	pins->nLength = 0;
	pins->uFlags &= ~(CHN_16BIT);
	for (UINT i=0; i<MAX_CHANNELS; i++)
	{
		if (Chn[i].pSample == pSample)
		{
			Chn[i].nPos = Chn[i].nLength = 0;
			Chn[i].pSample = Chn[i].pCurrentSample = NULL;
		}
	}
	FreeSample(pSample);
	return TRUE;
}
