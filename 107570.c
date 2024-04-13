void CSoundFile::ExtendedMODCommands(CHANNELINDEX nChn, ModCommand::PARAM param)
{
	ModChannel *pChn = &m_PlayState.Chn[nChn];
	uint8 command = param & 0xF0;
	param &= 0x0F;
	switch(command)
	{
	case 0x00:
		for(CHANNELINDEX chn = 0; chn < GetNumChannels(); chn++)
		{
			m_PlayState.Chn[chn].dwFlags.set(CHN_AMIGAFILTER, !(param & 1));
		}
		break;
	case 0x10:	if ((param) || (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2))) FinePortamentoUp(pChn, param); break;
	case 0x20:	if ((param) || (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2))) FinePortamentoDown(pChn, param); break;
	case 0x30:	pChn->dwFlags.set(CHN_GLISSANDO, param != 0); break;
	case 0x40:	pChn->nVibratoType = param & 0x07; break;
	case 0x50:	if(!m_SongFlags[SONG_FIRSTTICK])
				{
					break;
				}
				if(GetType() & (MOD_TYPE_MOD | MOD_TYPE_DIGI | MOD_TYPE_AMF0 | MOD_TYPE_MED))
				{
					pChn->nFineTune = MOD2XMFineTune(param);
					if(pChn->nPeriod && pChn->rowCommand.IsNote()) pChn->nPeriod = GetPeriodFromNote(pChn->nNote, pChn->nFineTune, pChn->nC5Speed);
				} else if(pChn->rowCommand.IsNote())
				{
					pChn->nFineTune = MOD2XMFineTune(param - 8);
					if(pChn->nPeriod) pChn->nPeriod = GetPeriodFromNote(pChn->nNote, pChn->nFineTune, pChn->nC5Speed);
				}
				break;
	case 0x70:	pChn->nTremoloType = param & 0x07; break;
	case 0x80:
		if(m_SongFlags[SONG_FIRSTTICK])
		{
			Panning(pChn, param, Pan4bit);
		}
		break;
	case 0x90:	RetrigNote(nChn, param); break;
	case 0xA0:	if ((param) || (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2))) FineVolumeUp(pChn, param, false); break;
	case 0xB0:	if ((param) || (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2))) FineVolumeDown(pChn, param, false); break;
	case 0xC0:	NoteCut(nChn, param, false); break;
	case 0xF0:
		if(GetType() == MOD_TYPE_MOD) // MOD: Invert Loop
		{
			pChn->nEFxSpeed = param;
			if(m_SongFlags[SONG_FIRSTTICK]) InvertLoop(pChn);
		} else // XM: Set Active Midi Macro
		{
			pChn->nActiveMacro = param;
		}
		break;
	}
}
