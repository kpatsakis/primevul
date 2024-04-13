CHANNELINDEX CSoundFile::GetNNAChannel(CHANNELINDEX nChn) const
{
	const ModChannel *pChn = &m_PlayState.Chn[nChn];

	const ModChannel *pi = &m_PlayState.Chn[m_nChannels];
	for (CHANNELINDEX i = m_nChannels; i < MAX_CHANNELS; i++, pi++) if (!pi->nLength) return i;
	if (!pChn->nFadeOutVol) return 0;

	CHANNELINDEX result = 0;
	uint32 vol = (1u << (14 + 9)) / 4u;	// 25%
	uint32 envpos = uint32_max;
	const ModChannel *pj = &m_PlayState.Chn[m_nChannels];
	for (CHANNELINDEX j = m_nChannels; j < MAX_CHANNELS; j++, pj++)
	{
		if (!pj->nFadeOutVol) return j;
		uint32 v = (pj->nRealVolume << 9) | pj->nVolume;
		if(pj->dwFlags[CHN_LOOP]) v >>= 1;
		if ((v < vol) || ((v == vol) && (pj->VolEnv.nEnvPosition > envpos)))
		{
			envpos = pj->VolEnv.nEnvPosition;
			vol = v;
			result = j;
		}
	}
	return result;
}
