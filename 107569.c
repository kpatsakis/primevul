void CSoundFile::ExtendedChannelEffect(ModChannel *pChn, uint32 param)
{
	if(!m_SongFlags[SONG_FIRSTTICK]) return;
	switch(param & 0x0F)
	{
	case 0x00:	pChn->dwFlags.reset(CHN_SURROUND);	break;
	case 0x01:	pChn->dwFlags.set(CHN_SURROUND); pChn->nPan = 128; break;

	case 0x08:
		pChn->dwFlags.reset(CHN_REVERB);
		pChn->dwFlags.set(CHN_NOREVERB);
		break;
	case 0x09:
		pChn->dwFlags.reset(CHN_NOREVERB);
		pChn->dwFlags.set(CHN_REVERB);
		break;
	case 0x0A:
		m_SongFlags.reset(SONG_SURROUNDPAN);
		break;
	case 0x0B:
		m_SongFlags.set(SONG_SURROUNDPAN);
		break;
	case 0x0C:
		m_SongFlags.reset(SONG_MPTFILTERMODE);
		break;
	case 0x0D:
		m_SongFlags.set(SONG_MPTFILTERMODE);
		break;
	case 0x0E:
		pChn->dwFlags.reset(CHN_PINGPONGFLAG);
		break;
	case 0x0F:
		if(pChn->position.IsZero() && pChn->nLength && (pChn->rowCommand.IsNote() || !pChn->dwFlags[CHN_LOOP]))
		{
			pChn->position.Set(pChn->nLength - 1, SamplePosition::fractMax);
		}
		pChn->dwFlags.set(CHN_PINGPONGFLAG);
		break;
	}
}
