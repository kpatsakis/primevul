void CSoundFile::ApplyInstrumentPanning(ModChannel *pChn, const ModInstrument *instr, const ModSample *smp) const
{
	int32 newPan = int32_min;
	if(instr != nullptr && instr->dwFlags[INS_SETPANNING])
		newPan = instr->nPan;
	if(smp != nullptr && smp->uFlags[CHN_PANNING])
		newPan = smp->nPan;

	if(newPan != int32_min)
	{
		pChn->nPan = newPan;
		if(m_playBehaviour[kPanOverride] && !m_SongFlags[SONG_SURROUNDPAN])
		{
			pChn->dwFlags.reset(CHN_SURROUND);
		}
	}
}
