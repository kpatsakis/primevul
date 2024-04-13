ROWINDEX CSoundFile::PatternLoop(ModChannel *pChn, uint32 param)
{
	if (param)
	{
		if(pChn->nPatternLoopCount)
		{
			pChn->nPatternLoopCount--;
			if(!pChn->nPatternLoopCount)
			{
				if(m_playBehaviour[kITPatternLoopTargetReset] || (GetType() == MOD_TYPE_S3M))
				{
					pChn->nPatternLoop = m_PlayState.m_nRow + 1;
				}

				return ROWINDEX_INVALID;
			}
		} else
		{

			if(!m_playBehaviour[kITFT2PatternLoop] && !(GetType() & (MOD_TYPE_MOD | MOD_TYPE_S3M)))
			{
				ModChannel *p = m_PlayState.Chn;
				for(CHANNELINDEX i = 0; i < GetNumChannels(); i++, p++) if (p != pChn)
				{
					if(p->nPatternLoopCount) return ROWINDEX_INVALID;
				}
			}
			pChn->nPatternLoopCount = static_cast<uint8>(param);
		}
		m_PlayState.m_nNextPatStartRow = pChn->nPatternLoop; // Nasty FT2 E60 bug emulation!
		return pChn->nPatternLoop;
	} else
	{
		pChn->nPatternLoop = m_PlayState.m_nRow;
	}
	return ROWINDEX_INVALID;
}
