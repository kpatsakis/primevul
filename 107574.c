void CSoundFile::FinePortamentoDown(ModChannel *pChn, ModCommand::PARAM param) const
{
	if(GetType() == MOD_TYPE_XM)
	{
		if(param) pChn->nOldFinePortaUpDown = (pChn->nOldFinePortaUpDown & 0xF0) | (param & 0x0F); else param = (pChn->nOldFinePortaUpDown & 0x0F);
	} else if(GetType() == MOD_TYPE_MT2)
	{
		if(param) pChn->nOldFinePortaUpDown = param; else param = pChn->nOldFinePortaUpDown;
	}

	if(pChn->isFirstTick)
	{
		if ((pChn->nPeriod) && (param))
		{
			if (m_SongFlags[SONG_LINEARSLIDES] && GetType() != MOD_TYPE_XM)
			{
				const auto oldPeriod = pChn->nPeriod;
				pChn->nPeriod = Util::muldivr(pChn->nPeriod, GetLinearSlideDownTable(this, param & 0x0F), 65536);
				if(oldPeriod == pChn->nPeriod)
				{
					if(!m_playBehaviour[kHertzInLinearMode] && pChn->nPeriod < Util::MaxValueOfType(pChn->nPeriod))
						pChn->nPeriod++;
					else if(m_playBehaviour[kHertzInLinearMode] && pChn->nPeriod > 1)
						pChn->nPeriod--;
				}
			} else
			{
				pChn->nPeriod += (int)(param * 4);
				if (pChn->nPeriod > 0xFFFF) pChn->nPeriod = 0xFFFF;
			}
		}
	}
}
