void CSoundFile::SetTempo(TEMPO param, bool setFromUI)
{
	const CModSpecifications &specs = GetModSpecifications();

	const TEMPO minTempo = (GetType() == MOD_TYPE_MDL) ? TEMPO(1, 0) : TEMPO(32, 0);

	if(setFromUI)
	{
		m_PlayState.m_nMusicTempo = Clamp(param, specs.GetTempoMin(), specs.GetTempoMax());
	} else if(param >= minTempo && m_SongFlags[SONG_FIRSTTICK] == !m_playBehaviour[kMODTempoOnSecondTick])
	{
		m_PlayState.m_nMusicTempo = std::min(param, specs.GetTempoMax());
	} else if(param < minTempo && !m_SongFlags[SONG_FIRSTTICK])
	{
		TEMPO tempDiff(param.GetInt() & 0x0F, 0);
		if((param.GetInt() & 0xF0) == 0x10)
			m_PlayState.m_nMusicTempo += tempDiff;
		else
			m_PlayState.m_nMusicTempo -= tempDiff;

		TEMPO tempoMin = specs.GetTempoMin(), tempoMax = specs.GetTempoMax();
		if(m_playBehaviour[kTempoClamp])	// clamp tempo correctly in compatible mode
		{
			tempoMax.Set(255);
		}
		Limit(m_PlayState.m_nMusicTempo, tempoMin, tempoMax);
	}
}
