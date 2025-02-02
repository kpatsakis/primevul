void CSoundFile::PortamentoUp(CHANNELINDEX nChn, ModCommand::PARAM param, const bool doFinePortamentoAsRegular)
{
	ModChannel *pChn = &m_PlayState.Chn[nChn];

	if(param)
	{
		if(!m_playBehaviour[kFT2PortaUpDownMemory])
			pChn->nOldPortaDown = param;
		pChn->nOldPortaUp = param;
	} else
	{
		param = pChn->nOldPortaUp;
	}

	const bool doFineSlides = !doFinePortamentoAsRegular && !(GetType() & (MOD_TYPE_MOD | MOD_TYPE_XM | MOD_TYPE_MT2 | MOD_TYPE_MED | MOD_TYPE_AMF0 | MOD_TYPE_DIGI | MOD_TYPE_STP | MOD_TYPE_DTM));

	MidiPortamento(nChn, param, doFineSlides);

	if(GetType() == MOD_TYPE_MPT && pChn->pModInstrument && pChn->pModInstrument->pTuning)
	{
		if(param >= 0xF0 && !doFinePortamentoAsRegular)
			PortamentoFineMPT(pChn, param - 0xF0);
		else if(param >= 0xE0 && !doFinePortamentoAsRegular)
			PortamentoExtraFineMPT(pChn, param - 0xE0);
		else
			PortamentoMPT(pChn, param);
		return;
	} else if(GetType() == MOD_TYPE_PLM)
	{
		pChn->nPortamentoDest = 1;
	}

	if (doFineSlides && param >= 0xE0)
	{
		if (param & 0x0F)
		{
			if ((param & 0xF0) == 0xF0)
			{
				FinePortamentoUp(pChn, param & 0x0F);
				return;
			} else if ((param & 0xF0) == 0xE0 && GetType() != MOD_TYPE_DBM)
			{
				ExtraFinePortamentoUp(pChn, param & 0x0F);
				return;
			}
		}
		if(GetType() != MOD_TYPE_DBM)
		{
			return;
		}
	}
	if(!pChn->isFirstTick || (m_PlayState.m_nMusicSpeed == 1 && m_playBehaviour[kSlidesAtSpeed1]) || GetType() == MOD_TYPE_669)
	{
		DoFreqSlide(pChn, -int(param) * 4);
	}
}
