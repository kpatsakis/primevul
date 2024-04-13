void CSoundFile::SetCurrentOrder(UINT nPos)
{
	while ((nPos < MAX_ORDERS) && (Order[nPos] == 0xFE)) nPos++;
	if ((nPos >= MAX_ORDERS) || (Order[nPos] >= MAX_PATTERNS)) return;
	for (UINT j=0; j<MAX_CHANNELS; j++)
	{
		Chn[j].nPeriod = 0;
		Chn[j].nNote = 0;
		Chn[j].nPortamentoDest = 0;
		Chn[j].nCommand = 0;
		Chn[j].nPatternLoopCount = 0;
		Chn[j].nPatternLoop = 0;
		Chn[j].nTremorCount = 0;
	}
	if (!nPos)
	{
		SetCurrentPos(0);
	} else
	{
		m_nNextPattern = nPos;
		m_nRow = m_nNextRow = 0;
		m_nPattern = 0;
		m_nTickCount = m_nMusicSpeed;
		m_nBufferCount = 0;
		m_nTotalCount = 0;
		m_nPatternDelay = 0;
		m_nFrameDelay = 0;
	}
	m_dwSongFlags &= ~(SONG_PATTERNLOOP|SONG_CPUVERYHIGH|SONG_FADINGSONG|SONG_ENDREACHED|SONG_GLOBALFADE);
}
