void CSoundFile::Panning(ModChannel *pChn, uint32 param, PanningType panBits) const
{
	if(m_playBehaviour[kMODIgnorePanning])
	{
		return;
	}
	if (!m_SongFlags[SONG_SURROUNDPAN] && (panBits == Pan8bit || m_playBehaviour[kPanOverride]))
	{
		pChn->dwFlags.reset(CHN_SURROUND);
	}
	if(panBits == Pan4bit)
	{
		pChn->nPan = (param * 256 + 8) / 15;
	} else if(panBits == Pan6bit)
	{
		if(param > 64) param = 64;
		pChn->nPan = param * 4;
	} else
	{
		if(!(GetType() & (MOD_TYPE_S3M | MOD_TYPE_DSM | MOD_TYPE_AMF | MOD_TYPE_MTM)))
		{
			pChn->nPan = param;
		} else
		{
			if(param <= 0x80)
			{
				pChn->nPan = param << 1;
			} else if(param == 0xA4)
			{
				pChn->dwFlags.set(CHN_SURROUND);
				pChn->nPan = 0x80;
			}
		}
	}

	pChn->dwFlags.set(CHN_FASTVOLRAMP);
	pChn->nRestorePanOnNewNote = 0;
	if(m_playBehaviour[kPanOverride])
	{
		pChn->nPanSwing = 0;
		pChn->nPanbrelloOffset = 0;
	}
}
