void CSoundFile::ExtendedS3MCommands(CHANNELINDEX nChn, ModCommand::PARAM param)
{
	ModChannel *pChn = &m_PlayState.Chn[nChn];
	uint8 command = param & 0xF0;
	param &= 0x0F;
	switch(command)
	{
	case 0x10:	pChn->dwFlags.set(CHN_GLISSANDO, param != 0); break;
	case 0x20:	if(!m_SongFlags[SONG_FIRSTTICK]) break;
				if(GetType() != MOD_TYPE_669)
				{
					pChn->nC5Speed = S3MFineTuneTable[param];
					pChn->nFineTune = MOD2XMFineTune(param);
					if (pChn->nPeriod) pChn->nPeriod = GetPeriodFromNote(pChn->nNote, pChn->nFineTune, pChn->nC5Speed);
				} else if(pChn->pModSample != nullptr)
				{
					pChn->nC5Speed = pChn->pModSample->nC5Speed + param * 80;
				}
				break;
	case 0x30:	if(GetType() == MOD_TYPE_S3M)
				{
					pChn->nVibratoType = param & 0x03;
				} else
				{
					if(m_playBehaviour[kITVibratoTremoloPanbrello])
						pChn->nVibratoType = (param < 0x04) ? param : 0;
					else
						pChn->nVibratoType = param & 0x07;
				}
				break;
	case 0x40:	if(GetType() == MOD_TYPE_S3M)
				{
					pChn->nTremoloType = param & 0x03;
				} else
				{
					if(m_playBehaviour[kITVibratoTremoloPanbrello])
						pChn->nTremoloType = (param < 0x04) ? param : 0;
					else
						pChn->nTremoloType = param & 0x07;
				}
				break;
	case 0x50:
				if(m_playBehaviour[kITVibratoTremoloPanbrello])
				{
					pChn->nPanbrelloType = (param < 0x04) ? param : 0;
					pChn->nPanbrelloPos = 0;
				} else
				{
					pChn->nPanbrelloType = param & 0x07;
				}
				break;
	case 0x60:
				if(m_SongFlags[SONG_FIRSTTICK] && m_PlayState.m_nTickCount == 0)
				{
					m_PlayState.m_nFrameDelay += param;
				}
				break;
	case 0x70:	if(!m_SongFlags[SONG_FIRSTTICK]) break;
				switch(param)
				{
				case 0:
				case 1:
				case 2:
					{
						ModChannel *bkp = &m_PlayState.Chn[m_nChannels];
						for (CHANNELINDEX i=m_nChannels; i<MAX_CHANNELS; i++, bkp++)
						{
							if (bkp->nMasterChn == nChn+1)
							{
								if (param == 1)
								{
									KeyOff(bkp);
								} else if (param == 2)
								{
									bkp->dwFlags.set(CHN_NOTEFADE);
								} else
								{
									bkp->dwFlags.set(CHN_NOTEFADE);
									bkp->nFadeOutVol = 0;
								}
#ifndef NO_PLUGINS
								const ModInstrument *pIns = bkp->pModInstrument;
								IMixPlugin *pPlugin;
								if(pIns != nullptr && pIns->nMixPlug && (pPlugin = m_MixPlugins[pIns->nMixPlug - 1].pMixPlugin) != nullptr)
								{
									pPlugin->MidiCommand(GetBestMidiChannel(nChn), pIns->nMidiProgram, pIns->wMidiBank, bkp->nNote + NOTE_MAX_SPECIAL, 0, nChn);
								}
#endif // NO_PLUGINS
							}
						}
					}
					break;
				case 3:		pChn->nNNA = NNA_NOTECUT; break;
				case 4:		pChn->nNNA = NNA_CONTINUE; break;
				case 5:		pChn->nNNA = NNA_NOTEOFF; break;
				case 6:		pChn->nNNA = NNA_NOTEFADE; break;
				case 7:		pChn->VolEnv.flags.reset(ENV_ENABLED); break;
				case 8:		pChn->VolEnv.flags.set(ENV_ENABLED); break;
				case 9:		pChn->PanEnv.flags.reset(ENV_ENABLED); break;
				case 10:	pChn->PanEnv.flags.set(ENV_ENABLED); break;
				case 11:	pChn->PitchEnv.flags.reset(ENV_ENABLED); break;
				case 12:	pChn->PitchEnv.flags.set(ENV_ENABLED); break;
				case 13:	// S7D: Enable pitch envelope, force to play as pitch envelope
				case 14:	// S7E: Enable pitch envelope, force to play as filter envelope
					if(GetType() == MOD_TYPE_MPT)
					{
						pChn->PitchEnv.flags.set(ENV_ENABLED);
						pChn->PitchEnv.flags.set(ENV_FILTER, param != 13);
					}
					break;
				}
				break;
	case 0x80:
		if(m_SongFlags[SONG_FIRSTTICK])
		{
			Panning(pChn, param, Pan4bit);
		}
		break;
	case 0x90:	ExtendedChannelEffect(pChn, param); break;
	case 0xA0:	if(m_SongFlags[SONG_FIRSTTICK])
				{
					pChn->nOldHiOffset = static_cast<uint8>(param);
					if (!m_playBehaviour[kITHighOffsetNoRetrig] && pChn->rowCommand.IsNote())
					{
						SmpLength pos = param << 16;
						if (pos < pChn->nLength) pChn->position.SetInt(pos);
					}
				}
				break;
	case 0xC0:
		if(param == 0)
		{
			if(GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT))
				param = 1;
			else if(GetType() == MOD_TYPE_S3M)
				return;
		}
		NoteCut(nChn, param, m_playBehaviour[kITSCxStopsSample] || GetType() == MOD_TYPE_S3M);
		break;
	case 0xF0:
		if(GetType() != MOD_TYPE_S3M)
		{
			pChn->nActiveMacro = static_cast<uint8>(param);
		}
		break;
	}
}
