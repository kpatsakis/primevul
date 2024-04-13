bool CSoundFile::ProcessEffects()
{
	ModChannel *pChn = m_PlayState.Chn;
	ROWINDEX nBreakRow = ROWINDEX_INVALID;		// Is changed if a break to row command is encountered
	ROWINDEX nPatLoopRow = ROWINDEX_INVALID;	// Is changed if a pattern loop jump-back is executed
	ORDERINDEX nPosJump = ORDERINDEX_INVALID;

	for(CHANNELINDEX nChn = 0; nChn < GetNumChannels(); nChn++, pChn++)
	{
		const uint32 tickCount = m_PlayState.m_nTickCount % (m_PlayState.m_nMusicSpeed + m_PlayState.m_nFrameDelay);
		uint32 instr = pChn->rowCommand.instr;
		ModCommand::VOLCMD volcmd = pChn->rowCommand.volcmd;
		uint32 vol = pChn->rowCommand.vol;
		ModCommand::COMMAND cmd = pChn->rowCommand.command;
		uint32 param = pChn->rowCommand.param;
		bool bPorta = pChn->rowCommand.IsPortamento();

		uint32 nStartTick = 0;
		pChn->isFirstTick = m_SongFlags[SONG_FIRSTTICK];

		if(pChn->rowCommand.note == NOTE_PC)
		{
#ifndef NO_PLUGINS
			const PLUGINDEX plug = pChn->rowCommand.instr;
			const PlugParamIndex plugparam = pChn->rowCommand.GetValueVolCol();
			const PlugParamValue value = pChn->rowCommand.GetValueEffectCol() / PlugParamValue(ModCommand::maxColumnValue);

			if(plug > 0 && plug <= MAX_MIXPLUGINS && m_MixPlugins[plug - 1].pMixPlugin)
				m_MixPlugins[plug-1].pMixPlugin->SetParameter(plugparam, value);
#endif // NO_PLUGINS
		}

		if(pChn->rowCommand.note == NOTE_PCS || (cmd == CMD_NONE && pChn->m_plugParamValueStep != 0))
		{
#ifndef NO_PLUGINS
			const bool isFirstTick = m_SongFlags[SONG_FIRSTTICK];
			if(isFirstTick)
				pChn->m_RowPlug = pChn->rowCommand.instr;
			const PLUGINDEX nPlug = pChn->m_RowPlug;
			const bool hasValidPlug = (nPlug > 0 && nPlug <= MAX_MIXPLUGINS && m_MixPlugins[nPlug-1].pMixPlugin);
			if(hasValidPlug)
			{
				if(isFirstTick)
					pChn->m_RowPlugParam = ModCommand::GetValueVolCol(pChn->rowCommand.volcmd, pChn->rowCommand.vol);
				const PlugParamIndex plugparam = pChn->m_RowPlugParam;
				if(isFirstTick)
				{
					PlugParamValue targetvalue = ModCommand::GetValueEffectCol(pChn->rowCommand.command, pChn->rowCommand.param) / PlugParamValue(ModCommand::maxColumnValue);
					pChn->m_plugParamTargetValue = targetvalue;
					pChn->m_plugParamValueStep = (targetvalue - m_MixPlugins[nPlug-1].pMixPlugin->GetParameter(plugparam)) / float(GetNumTicksOnCurrentRow());
				}
				if(m_PlayState.m_nTickCount + 1 == GetNumTicksOnCurrentRow())
				{	// On last tick, set parameter exactly to target value.
					m_MixPlugins[nPlug-1].pMixPlugin->SetParameter(plugparam, pChn->m_plugParamTargetValue);
				}
				else
					m_MixPlugins[nPlug-1].pMixPlugin->ModifyParameter(plugparam, pChn->m_plugParamValueStep);
			}
#endif // NO_PLUGINS
		}

		if(ModCommand::IsPcNote(pChn->rowCommand.note))
		{
			pChn->ClearRowCmd();
			instr = 0;
			volcmd = VOLCMD_NONE;
			vol = 0;
			cmd = CMD_NONE;
			param = 0;
			bPorta = false;
		}

		if(!m_SongFlags[SONG_FIRSTTICK])
		{
			InvertLoop(&m_PlayState.Chn[nChn]);
		} else
		{
			if(instr) m_PlayState.Chn[nChn].nEFxOffset = 0;
		}

		if (cmd == CMD_DELAYCUT)
		{
			nStartTick = (param & 0xF0) >> 4;
			const uint32 cutAtTick = nStartTick + (param & 0x0F);
			NoteCut(nChn, cutAtTick, m_playBehaviour[kITSCxStopsSample]);
		} else if ((cmd == CMD_MODCMDEX) || (cmd == CMD_S3MCMDEX))
		{
			if ((!param) && (GetType() & (MOD_TYPE_S3M|MOD_TYPE_IT|MOD_TYPE_MPT)))
				param = pChn->nOldCmdEx;
			else
				pChn->nOldCmdEx = static_cast<ModCommand::PARAM>(param);

			if ((param & 0xF0) == 0xD0)
			{
				nStartTick = param & 0x0F;
				if(nStartTick == 0)
				{
					if(GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT))
						nStartTick = 1;
					else if(GetType() == MOD_TYPE_S3M)
						continue;
				} else if(nStartTick >= (m_PlayState.m_nMusicSpeed + m_PlayState.m_nFrameDelay) && m_playBehaviour[kITOutOfRangeDelay])
				{
					if(instr)
					{
						pChn->nNewIns = static_cast<ModCommand::INSTR>(instr);
					}
					continue;
				}
			} else if(m_SongFlags[SONG_FIRSTTICK])
			{
				if((((param & 0xF0) == 0x60 && cmd == CMD_MODCMDEX)
					|| ((param & 0xF0) == 0xB0 && cmd == CMD_S3MCMDEX))
					&& !(m_playBehaviour[kST3NoMutedChannels] && ChnSettings[nChn].dwFlags[CHN_MUTE]))	// not even effects are processed on muted S3M channels
				{
					ROWINDEX nloop = PatternLoop(pChn, param & 0x0F);
					if (nloop != ROWINDEX_INVALID)
					{
						if(nBreakRow != ROWINDEX_INVALID && m_playBehaviour[kFT2PatternLoopWithJumps])
						{
							nBreakRow = nloop;
						}

						nPatLoopRow = nloop;
					}

					if(GetType() == MOD_TYPE_S3M)
					{
						for (CHANNELINDEX i = 0; i < GetNumChannels(); i++)
						{
							m_PlayState.Chn[i].nPatternLoop = pChn->nPatternLoop;
							m_PlayState.Chn[i].nPatternLoopCount = pChn->nPatternLoopCount;
						}
					}
				} else if ((param & 0xF0) == 0xE0)
				{
					if(!(GetType() & (MOD_TYPE_S3M | MOD_TYPE_IT | MOD_TYPE_MPT)) || !m_PlayState.m_nPatternDelay)
					{
						if(!(GetType() & (MOD_TYPE_S3M)) || (param & 0x0F) != 0)
						{
							m_PlayState.m_nPatternDelay = 1 + (param & 0x0F);
						}
					}
				}
			}
		}

		if(GetType() == MOD_TYPE_MTM && cmd == CMD_MODCMDEX && (param & 0xF0) == 0xD0)
		{
			nStartTick = 0;
			param = 0x90 | (param & 0x0F);
		}

		if(nStartTick != 0 && pChn->rowCommand.note == NOTE_KEYOFF && pChn->rowCommand.volcmd == VOLCMD_PANNING && m_playBehaviour[kFT2PanWithDelayedNoteOff])
		{
			pChn->rowCommand.volcmd = VOLCMD_NONE;
		}

		bool triggerNote = (m_PlayState.m_nTickCount == nStartTick);	// Can be delayed by a note delay effect
		if(m_playBehaviour[kFT2OutOfRangeDelay] && nStartTick >= m_PlayState.m_nMusicSpeed)
		{
			triggerNote = false;
		} else if(m_playBehaviour[kRowDelayWithNoteDelay] && nStartTick > 0 && tickCount == nStartTick)
		{
			triggerNote = true;
		}

		if(m_playBehaviour[kITFirstTickHandling])
		{
			pChn->isFirstTick = tickCount == nStartTick;
		}

		if(m_playBehaviour[kFT2PortaDelay] && nStartTick != 0)
		{
			bPorta = false;
		}

		if(m_SongFlags[SONG_PT_MODE] && instr && !m_PlayState.m_nTickCount)
		{
			pChn->proTrackerOffset = 0;
			if(!triggerNote && pChn->IsSamplePlaying())
			{
				pChn->nNewIns = static_cast<ModCommand::INSTR>(instr);
				if(instr <= GetNumSamples())
				{
					pChn->nVolume = Samples[instr].nVolume;
					pChn->nFineTune = Samples[instr].nFineTune;
				}
			}
		}

		if(triggerNote)
		{
			ModCommand::NOTE note = pChn->rowCommand.note;
			if(instr) pChn->nNewIns = static_cast<ModCommand::INSTR>(instr);

			if(ModCommand::IsNote(note) && m_playBehaviour[kFT2Transpose])
			{
				int transpose = pChn->nTranspose;
				if(instr && !bPorta)
				{
					SAMPLEINDEX sample = SAMPLEINDEX_INVALID;
					if(GetNumInstruments())
					{
						if(instr <= GetNumInstruments() && Instruments[instr] != nullptr)
						{
							sample = Instruments[instr]->Keyboard[note - NOTE_MIN];
						}
					} else
					{
						sample = static_cast<SAMPLEINDEX>(instr);
					}
					if(sample <= GetNumSamples())
					{
						transpose = GetSample(sample).RelativeTone;
					}
				}

				const int computedNote = note + transpose;
				if((computedNote < NOTE_MIN + 11 || computedNote > NOTE_MIN + 130))
				{
					note = NOTE_NONE;
				}
			} else if((GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT)) && GetNumInstruments() != 0 && ModCommand::IsNoteOrEmpty(static_cast<ModCommand::NOTE>(note)))
			{
				INSTRUMENTINDEX instrToCheck = static_cast<INSTRUMENTINDEX>((instr != 0) ? instr : pChn->nOldIns);
				if(instrToCheck != 0 && (instrToCheck > GetNumInstruments() || Instruments[instrToCheck] == nullptr))
				{
					note = NOTE_NONE;
					instr = 0;
				}
			}

			if(cmd == CMD_KEYOFF && param == 0 && m_playBehaviour[kFT2KeyOff])
			{
				note = NOTE_NONE;
				instr = 0;
			}

			bool retrigEnv = note == NOTE_NONE && instr != 0;

			bool reloadSampleSettings = (m_playBehaviour[kFT2ReloadSampleSettings] && instr != 0);
			bool keepInstr = (GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT)) || (m_playBehaviour[kMODSampleSwap] && !pChn->IsSamplePlaying() && pChn->pModSample != nullptr && !pChn->pModSample->HasSampleData());

			if (GetType() & (MOD_TYPE_XM | MOD_TYPE_MT2))
			{

				if(note == NOTE_KEYOFF
					&& ((!instr && volcmd != VOLCMD_VOLUME && cmd != CMD_VOLUME) || !m_playBehaviour[kFT2KeyOff])
					&& (pChn->pModInstrument == nullptr || !pChn->pModInstrument->VolEnv.dwFlags[ENV_ENABLED]))
				{
					pChn->dwFlags.set(CHN_FASTVOLRAMP);
					pChn->nVolume = 0;
					note = NOTE_NONE;
					instr = 0;
					retrigEnv = false;
					if(m_SongFlags[SONG_FIRSTTICK] && m_playBehaviour[kFT2NoteOffFlags])
						pChn->dwFlags.set(CHN_NOTEFADE);
				} else if(m_playBehaviour[kFT2RetrigWithNoteDelay] && !m_SongFlags[SONG_FIRSTTICK])
				{

					retrigEnv = true;

					bPorta = false;

					if(note == NOTE_NONE)
					{
						note = static_cast<ModCommand::NOTE>(pChn->nNote - pChn->nTranspose);
					} else if(note >= NOTE_MIN_SPECIAL)
					{
						note = NOTE_NONE;
						keepInstr = false;
						reloadSampleSettings = true;
					} else
					{
						keepInstr = true;
						reloadSampleSettings = true;
					}
				}
			}

			if((retrigEnv && !m_playBehaviour[kFT2ReloadSampleSettings]) || reloadSampleSettings)
			{
				const ModSample *oldSample = nullptr;

				if(GetNumInstruments())
				{
					oldSample = pChn->pModSample;
				} else if (instr <= GetNumSamples())
				{
					oldSample = &Samples[instr];
				}

				if(oldSample != nullptr)
				{
					if(!oldSample->uFlags[SMP_NODEFAULTVOLUME])
						pChn->nVolume = oldSample->nVolume;
					if(reloadSampleSettings)
					{
						pChn->nPan = oldSample->nPan;
					}
				}
			}

			if(m_playBehaviour[kFT2Tremor] && instr != 0)
			{
				pChn->nTremorCount = 0x20;
			}

			if(retrigEnv) //Case: instrument with no note data.
			{
				if(m_playBehaviour[kITInstrWithoutNote] || GetType() == MOD_TYPE_PLM)
				{
					bool triggerAfterSmpEnd = m_playBehaviour[kITMultiSampleInstrumentNumber] && !pChn->IsSamplePlaying();
					if(GetNumInstruments())
					{
						if(instr <= GetNumInstruments() && (pChn->pModInstrument != Instruments[instr] || triggerAfterSmpEnd))
							note = pChn->nNote;
					} else
					{
						if(instr < MAX_SAMPLES && (pChn->pModSample != &Samples[instr] || triggerAfterSmpEnd))
							note = pChn->nNote;
					}
				}

				if (GetNumInstruments() && (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2)))
				{
					pChn->ResetEnvelopes();
					pChn->dwFlags.set(CHN_FASTVOLRAMP);
					pChn->dwFlags.reset(CHN_NOTEFADE);
					pChn->nAutoVibDepth = 0;
					pChn->nAutoVibPos = 0;
					pChn->nFadeOutVol = 65536;
					if(m_playBehaviour[kFT2NoteOffFlags])
						pChn->dwFlags.reset(CHN_KEYOFF);
				}
				if (!keepInstr) instr = 0;
			}

			if (note >= NOTE_MIN_SPECIAL)
			{
				if(m_playBehaviour[kITInstrWithNoteOff] && instr)
				{
					SAMPLEINDEX smp = static_cast<SAMPLEINDEX>(instr);
					if(GetNumInstruments())
					{
						smp = 0;
						if(instr <= GetNumInstruments() && Instruments[instr] != nullptr && ModCommand::IsNote(pChn->nLastNote))
						{
							smp = Instruments[instr]->Keyboard[pChn->nLastNote - NOTE_MIN];
						}
					}
					if(smp > 0 && smp <= GetNumSamples() && !Samples[smp].uFlags[SMP_NODEFAULTVOLUME])
						pChn->nVolume = Samples[smp].nVolume;
				}
				instr = 0;
			}

			if(ModCommand::IsNote(note))
			{
				pChn->nNewNote = pChn->nLastNote = note;

				if(!bPorta)
				{
					CheckNNA(nChn, instr, note, false);
				}
			}

			if(note)
			{
				if(pChn->nRestorePanOnNewNote > 0)
				{
					pChn->nPan = pChn->nRestorePanOnNewNote - 1;
					pChn->nRestorePanOnNewNote = 0;
				}
				if(pChn->nRestoreResonanceOnNewNote > 0)
				{
					pChn->nResonance = pChn->nRestoreResonanceOnNewNote - 1;
					pChn->nRestoreResonanceOnNewNote = 0;
				}
				if(pChn->nRestoreCutoffOnNewNote > 0)
				{
					pChn->nCutOff = pChn->nRestoreCutoffOnNewNote - 1;
					pChn->nRestoreCutoffOnNewNote = 0;
				}
			}

			if(instr)
			{
				const ModSample *oldSample = pChn->pModSample;

				InstrumentChange(pChn, instr, bPorta, true);
				if(GetType() == MOD_TYPE_MOD)
				{
					if(!bPorta || !m_playBehaviour[kMODSampleSwap]) pChn->nNewIns = 0;
				} else
				{
					if(!m_playBehaviour[kITInstrWithNoteOff] || ModCommand::IsNote(note)) pChn->nNewIns = 0;
				}

				if(m_playBehaviour[kITPortamentoSwapResetsPos])
				{
					if(ModCommand::IsNote(note) && oldSample != pChn->pModSample)
					{
						pChn->position.Set(0);
					}
				} else if ((GetType() & (MOD_TYPE_S3M | MOD_TYPE_IT | MOD_TYPE_MPT) && oldSample != pChn->pModSample && ModCommand::IsNote(note)))
				{
					bPorta = false;
				} else if(m_playBehaviour[kMODSampleSwap] && pChn->increment.IsZero())
				{
					pChn->position.Set(0);
				}
			}
			if (note)
			{
				if ((!instr) && (pChn->nNewIns) && (note < 0x80))
				{
					InstrumentChange(pChn, pChn->nNewIns, bPorta, pChn->pModSample == nullptr && pChn->pModInstrument == nullptr, !(GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2)));
					pChn->nNewIns = 0;
				}
				NoteChange(pChn, note, bPorta, !(GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2)));
				if ((bPorta) && (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2)) && (instr))
				{
					pChn->dwFlags.set(CHN_FASTVOLRAMP);
					pChn->ResetEnvelopes();
					pChn->nAutoVibDepth = 0;
					pChn->nAutoVibPos = 0;
				}
			}
			if (volcmd == VOLCMD_VOLUME)
			{
				if (vol > 64) vol = 64;
				pChn->nVolume = vol << 2;
				pChn->dwFlags.set(CHN_FASTVOLRAMP);
			} else
			if (volcmd == VOLCMD_PANNING)
			{
				Panning(pChn, vol, Pan6bit);
			}

#ifndef NO_PLUGINS
			if (m_nInstruments) ProcessMidiOut(nChn);
#endif // NO_PLUGINS
		}

		if(m_playBehaviour[kST3NoMutedChannels] && ChnSettings[nChn].dwFlags[CHN_MUTE])	// not even effects are processed on muted S3M channels
			continue;

		/*	A few notes, paraphrased from ITTECH.TXT by Storlek (creator of schismtracker):
			Ex/Fx/Gx are shared with Exx/Fxx/Gxx; Ex/Fx are 4x the 'normal' slide value
			Gx is linked with Ex/Fx if Compat Gxx is off, just like Gxx is with Exx/Fxx
			Gx values: 1, 4, 8, 16, 32, 64, 96, 128, 255
			Ax/Bx/Cx/Dx values are used directly (i.e. D9 == D09), and are NOT shared with Dxx
			(value is stored into nOldVolParam and used by A0/B0/C0/D0)
			Hx uses the same value as Hxx and Uxx, and affects the *depth*
			so... hxx = (hx | (oldhxx & 0xf0))  ???
			TODO is this done correctly?
		*/
		bool doVolumeColumn = m_PlayState.m_nTickCount >= nStartTick;
		if(m_playBehaviour[kFT2VolColDelay] && nStartTick != 0)
		{
			doVolumeColumn = m_PlayState.m_nTickCount != 0 && (m_PlayState.m_nTickCount != nStartTick || (pChn->rowCommand.instr == 0 && volcmd != VOLCMD_TONEPORTAMENTO));
		}
		if(volcmd > VOLCMD_PANNING && doVolumeColumn)
		{
			if (volcmd == VOLCMD_TONEPORTAMENTO)
			{
				uint32 porta = 0;
				if(GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT | MOD_TYPE_AMS | MOD_TYPE_AMS2 | MOD_TYPE_DMF | MOD_TYPE_DBM | MOD_TYPE_IMF | MOD_TYPE_PSM | MOD_TYPE_J2B | MOD_TYPE_ULT | MOD_TYPE_OKT | MOD_TYPE_MT2 | MOD_TYPE_MDL))
				{
					porta = ImpulseTrackerPortaVolCmd[vol & 0x0F];
				} else
				{
					if(cmd == CMD_TONEPORTAMENTO && GetType() == MOD_TYPE_XM)
					{
						cmd = CMD_NONE;
						vol *= 2;
					}
					porta = vol << 4;

					if(m_playBehaviour[kFT2PortaDelay] && nStartTick != 0)
					{
						porta = 0;
					}
				}
				TonePortamento(pChn, porta);
			} else
			{
				if(m_playBehaviour[kFT2VolColMemory] && vol == 0)
				{
					switch(volcmd)
					{
					case VOLCMD_VOLUME:
					case VOLCMD_PANNING:
					case VOLCMD_VIBRATODEPTH:
						break;
					case VOLCMD_PANSLIDELEFT:
						if(!m_SongFlags[SONG_FIRSTTICK])
						{
							pChn->nPan = 0;
						}
						MPT_FALLTHROUGH;
					default:
						volcmd = VOLCMD_NONE;
					}

				} else if(!m_playBehaviour[kITVolColMemory])
				{
					if(vol) pChn->nOldVolParam = static_cast<ModCommand::PARAM>(vol); else vol = pChn->nOldVolParam;
				}

				switch(volcmd)
				{
				case VOLCMD_VOLSLIDEUP:
				case VOLCMD_VOLSLIDEDOWN:
					if(vol == 0 && m_playBehaviour[kITVolColMemory])
					{
						vol = pChn->nOldVolParam;
						if(vol == 0)
							break;
					} else
					{
						pChn->nOldVolParam = static_cast<ModCommand::PARAM>(vol);
					}
					VolumeSlide(pChn, static_cast<ModCommand::PARAM>(volcmd == VOLCMD_VOLSLIDEUP ? (vol << 4) : vol));
					break;

				case VOLCMD_FINEVOLUP:
					if(m_PlayState.m_nTickCount == nStartTick || !m_playBehaviour[kITVolColMemory])
					{
						FineVolumeUp(pChn, static_cast<ModCommand::PARAM>(vol), m_playBehaviour[kITVolColMemory]);
					}
					break;

				case VOLCMD_FINEVOLDOWN:
					if(m_PlayState.m_nTickCount == nStartTick || !m_playBehaviour[kITVolColMemory])
					{
						FineVolumeDown(pChn, static_cast<ModCommand::PARAM>(vol), m_playBehaviour[kITVolColMemory]);
					}
					break;

				case VOLCMD_VIBRATOSPEED:
					if(m_playBehaviour[kFT2VolColVibrato])
						pChn->nVibratoSpeed = vol & 0x0F;
					else
						Vibrato(pChn, vol << 4);
					break;

				case VOLCMD_VIBRATODEPTH:
					Vibrato(pChn, vol);
					break;

				case VOLCMD_PANSLIDELEFT:
					PanningSlide(pChn, static_cast<ModCommand::PARAM>(vol), !m_playBehaviour[kFT2VolColMemory]);
					break;

				case VOLCMD_PANSLIDERIGHT:
					PanningSlide(pChn, static_cast<ModCommand::PARAM>(vol << 4), !m_playBehaviour[kFT2VolColMemory]);
					break;

				case VOLCMD_PORTAUP:
					PortamentoUp(nChn, static_cast<ModCommand::PARAM>(vol << 2), m_playBehaviour[kITVolColFinePortamento]);
					break;

				case VOLCMD_PORTADOWN:
					PortamentoDown(nChn, static_cast<ModCommand::PARAM>(vol << 2), m_playBehaviour[kITVolColFinePortamento]);
					break;

				case VOLCMD_OFFSET:
					if (triggerNote && pChn->pModSample && vol <= CountOf(pChn->pModSample->cues))
					{
						SmpLength offset;
						if(vol == 0)
							offset = pChn->oldOffset;
						else
							offset = pChn->oldOffset = pChn->pModSample->cues[vol - 1];
						SampleOffset(*pChn, offset);
					}
					break;
				}
			}
		}

		if(cmd != CMD_NONE) switch (cmd)
		{
		case CMD_VOLUME:
			if(m_SongFlags[SONG_FIRSTTICK])
			{
				pChn->nVolume = (param < 64) ? param * 4 : 256;
				pChn->dwFlags.set(CHN_FASTVOLRAMP);
			}
			break;

		case CMD_PORTAMENTOUP:
			if ((!param) && (GetType() & MOD_TYPE_MOD)) break;
			PortamentoUp(nChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_PORTAMENTODOWN:
			if ((!param) && (GetType() & MOD_TYPE_MOD)) break;
			PortamentoDown(nChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_VOLUMESLIDE:
			if (param || (GetType() != MOD_TYPE_MOD)) VolumeSlide(pChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_TONEPORTAMENTO:
			TonePortamento(pChn, param);
			break;

		case CMD_TONEPORTAVOL:
			if ((param) || (GetType() != MOD_TYPE_MOD)) VolumeSlide(pChn, static_cast<ModCommand::PARAM>(param));
			TonePortamento(pChn, 0);
			break;

		case CMD_VIBRATO:
			Vibrato(pChn, param);
			break;

		case CMD_VIBRATOVOL:
			if ((param) || (GetType() != MOD_TYPE_MOD)) VolumeSlide(pChn, static_cast<ModCommand::PARAM>(param));
			Vibrato(pChn, 0);
			break;

		case CMD_SPEED:
			if(m_SongFlags[SONG_FIRSTTICK])
				SetSpeed(m_PlayState, param);
			break;

		case CMD_TEMPO:
			if(m_playBehaviour[kMODVBlankTiming])
			{
				if(m_SongFlags[SONG_FIRSTTICK] && param != 0) SetSpeed(m_PlayState, param);
				break;
			}
			{
				param = CalculateXParam(m_PlayState.m_nPattern, m_PlayState.m_nRow, nChn);
				if (GetType() & (MOD_TYPE_S3M|MOD_TYPE_IT|MOD_TYPE_MPT))
				{
					if (param) pChn->nOldTempo = static_cast<ModCommand::PARAM>(param); else param = pChn->nOldTempo;
				}
				TEMPO t(param, 0);
				LimitMax(t, GetModSpecifications().GetTempoMax());
				SetTempo(t);
			}
			break;

		case CMD_OFFSET:
			if (triggerNote)
			{
				if(bPorta && GetType() == MOD_TYPE_XM)
				{
					break;
				}
				bool isExtended = false;
				SmpLength offset = CalculateXParam(m_PlayState.m_nPattern, m_PlayState.m_nRow, nChn, &isExtended);
				if(!isExtended)
				{
					offset <<= 8;
					if (offset) pChn->oldOffset = offset; else offset = pChn->oldOffset;
					offset += static_cast<SmpLength>(pChn->nOldHiOffset) << 16;
				}
				SampleOffset(*pChn, offset);
			}
			break;

		case CMD_OFFSETPERCENTAGE:
			if(triggerNote)
			{
				SampleOffset(*pChn, Util::muldiv_unsigned(pChn->nLength, param, 255));
			}
			break;

		case CMD_ARPEGGIO:
			if(m_PlayState.m_nTickCount) break;
			if((!pChn->nPeriod || !pChn->nNote)
				&& (pChn->pModInstrument == nullptr || !pChn->pModInstrument->HasValidMIDIChannel())	// Plugin arpeggio
				&& !m_playBehaviour[kITArpeggio] && (GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT))) break;
			if (!param && (GetType() & (MOD_TYPE_XM | MOD_TYPE_MOD))) break;	// Only important when editing MOD/XM files (000 effects are removed when loading files where this means "no effect")
			pChn->nCommand = CMD_ARPEGGIO;
			if (param) pChn->nArpeggio = static_cast<ModCommand::PARAM>(param);
			break;

		case CMD_RETRIG:
			if (GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2))
			{
				if (!(param & 0xF0)) param |= pChn->nRetrigParam & 0xF0;
				if (!(param & 0x0F)) param |= pChn->nRetrigParam & 0x0F;
				param |= 0x100; // increment retrig count on first row
			}
			if(m_playBehaviour[kITRetrigger])
			{
				if (param) pChn->nRetrigParam = static_cast<uint8>(param & 0xFF);
				RetrigNote(nChn, pChn->nRetrigParam, (volcmd == VOLCMD_OFFSET) ? vol + 1 : 0);
			} else
			{
				if (param) pChn->nRetrigParam = static_cast<uint8>(param & 0xFF); else param = pChn->nRetrigParam;
				RetrigNote(nChn, param, (volcmd == VOLCMD_OFFSET) ? vol + 1 : 0);
			}
			break;

		case CMD_TREMOR:
			if(!m_SongFlags[SONG_FIRSTTICK])
			{
				break;
			}

			if(m_playBehaviour[kITTremor])
			{
				if(param && !m_SongFlags[SONG_ITOLDEFFECTS])
				{
					if(param & 0xF0) param -= 0x10;
					if(param & 0x0F) param -= 0x01;
				}
				pChn->nTremorCount |= 0x80; // set on/off flag
			} else if(m_playBehaviour[kFT2Tremor])
			{
				pChn->nTremorCount |= 0x80; // set on/off flag
			}

			pChn->nCommand = CMD_TREMOR;
			if (param) pChn->nTremorParam = static_cast<ModCommand::PARAM>(param);

			break;

		case CMD_GLOBALVOLUME:
			if(!m_SongFlags[SONG_FIRSTTICK])
				break;


			if (!(GetType() & GLOBALVOL_7BIT_FORMATS)) param *= 2;

			if(param <= 128)
			{
				m_PlayState.m_nGlobalVolume = param * 2;
			} else if(!(GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT | MOD_TYPE_S3M)))
			{
				m_PlayState.m_nGlobalVolume = 256;
			}
			break;

		case CMD_GLOBALVOLSLIDE:
			if(m_playBehaviour[kPerChannelGlobalVolSlide])
				GlobalVolSlide(static_cast<ModCommand::PARAM>(param), pChn->nOldGlobalVolSlide);
			else
				GlobalVolSlide(static_cast<ModCommand::PARAM>(param), m_PlayState.Chn[0].nOldGlobalVolSlide);
			break;

		case CMD_PANNING8:
			if(m_SongFlags[SONG_FIRSTTICK])
			{
				Panning(pChn, param, Pan8bit);
			}
			break;

		case CMD_PANNINGSLIDE:
			PanningSlide(pChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_TREMOLO:
			Tremolo(pChn, param);
			break;

		case CMD_FINEVIBRATO:
			FineVibrato(pChn, param);
			break;

		case CMD_MODCMDEX:
			ExtendedMODCommands(nChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_S3MCMDEX:
			if(m_playBehaviour[kST3EffectMemory] && param == 0)
			{
				param = pChn->nArpeggio;	// S00 uses the last non-zero effect parameter as memory, like other effects including Arpeggio, so we "borrow" our memory there.
			}
			ExtendedS3MCommands(nChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_KEYOFF:
			if(m_playBehaviour[kFT2KeyOff])
			{
				if (m_PlayState.m_nTickCount == param)
				{
					if(pChn->pModInstrument == nullptr || !pChn->pModInstrument->VolEnv.dwFlags[ENV_ENABLED])
					{
						if(param == 0 && (pChn->rowCommand.instr || pChn->rowCommand.volcmd != VOLCMD_NONE)) // FT2 is weird....
						{
							pChn->dwFlags.set(CHN_NOTEFADE);
						}
						else
						{
							pChn->dwFlags.set(CHN_FASTVOLRAMP);
							pChn->nVolume = 0;
						}
					}
					KeyOff(pChn);
				}
			}
			else
			{
				if(m_SongFlags[SONG_FIRSTTICK])
					KeyOff(pChn);
			}
			break;

		case CMD_XFINEPORTAUPDOWN:
			switch(param & 0xF0)
			{
			case 0x10: ExtraFinePortamentoUp(pChn, param & 0x0F); break;
			case 0x20: ExtraFinePortamentoDown(pChn, param & 0x0F); break;
			case 0x50:
			case 0x60:
			case 0x70:
			case 0x90:
			case 0xA0:
				if(!m_playBehaviour[kFT2RestrictXCommand]) ExtendedS3MCommands(nChn, static_cast<ModCommand::PARAM>(param));
				break;
			}
			break;

		case CMD_CHANNELVOLUME:
			if(!m_SongFlags[SONG_FIRSTTICK]) break;
			if (param <= 64)
			{
				pChn->nGlobalVol = param;
				pChn->dwFlags.set(CHN_FASTVOLRAMP);
			}
			break;

		case CMD_CHANNELVOLSLIDE:
			ChannelVolSlide(pChn, static_cast<ModCommand::PARAM>(param));
			break;

		case CMD_PANBRELLO:
			Panbrello(pChn, param);
			break;

		case CMD_SETENVPOSITION:
			if(m_SongFlags[SONG_FIRSTTICK])
			{
				pChn->VolEnv.nEnvPosition = param;

				if(!m_playBehaviour[kFT2SetPanEnvPos] || pChn->VolEnv.flags[ENV_SUSTAIN])
				{
					pChn->PanEnv.nEnvPosition = param;
					pChn->PitchEnv.nEnvPosition = param;
				}

			}
			break;

		case CMD_POSITIONJUMP:
			m_PlayState.m_nNextPatStartRow = 0; // FT2 E60 bug
			nPosJump = static_cast<ORDERINDEX>(CalculateXParam(m_PlayState.m_nPattern, m_PlayState.m_nRow, nChn));

			if((GetType() & (MOD_TYPE_MOD | MOD_TYPE_XM)) && nBreakRow != ROWINDEX_INVALID)
			{
				nBreakRow = 0;
			}
			break;

		case CMD_PATTERNBREAK:
			{
				ROWINDEX row = PatternBreak(m_PlayState, nChn, static_cast<ModCommand::PARAM>(param));
				if(row != ROWINDEX_INVALID)
				{
					nBreakRow = row;
					if(m_SongFlags[SONG_PATTERNLOOP])
					{
						nPosJump = m_PlayState.m_nCurrentOrder;
					}
				}
			}
			break;

		case CMD_NOTESLIDEUP:
		case CMD_NOTESLIDEDOWN:
		case CMD_NOTESLIDEUPRETRIG:
		case CMD_NOTESLIDEDOWNRETRIG:
			NoteSlide(pChn, param, cmd == CMD_NOTESLIDEUP || cmd == CMD_NOTESLIDEUPRETRIG, cmd == CMD_NOTESLIDEUPRETRIG || cmd == CMD_NOTESLIDEDOWNRETRIG);
			break;

		case CMD_REVERSEOFFSET:
			ReverseSampleOffset(*pChn, static_cast<ModCommand::PARAM>(param));
			break;

#ifndef NO_PLUGINS
		case CMD_DBMECHO:
			if(m_PlayState.m_nTickCount == 0)
			{
				uint32 chns = (param >> 4), enable = (param & 0x0F);
				if(chns > 1 || enable > 2)
				{
					break;
				}
				CHANNELINDEX firstChn = nChn, lastChn = nChn;
				if(chns == 1)
				{
					firstChn = 0;
					lastChn = m_nChannels - 1;
				}
				for(CHANNELINDEX c = firstChn; c <= lastChn; c++)
				{
					ChnSettings[c].dwFlags.set(CHN_NOFX, enable == 1);
					m_PlayState.Chn[c].dwFlags.set(CHN_NOFX, enable == 1);
				}
			}
			break;
#endif // NO_PLUGINS
		}

		if(m_playBehaviour[kST3EffectMemory] && param != 0)
		{
			UpdateS3MEffectMemory(pChn, static_cast<ModCommand::PARAM>(param));
		}

		if(pChn->rowCommand.instr)
		{
			pChn->nOldIns = pChn->rowCommand.instr;
		}

	} // for(...) end

	if(m_SongFlags[SONG_FIRSTTICK])
	{
		const bool doPatternLoop = (nPatLoopRow != ROWINDEX_INVALID);
		const bool doBreakRow = (nBreakRow != ROWINDEX_INVALID);
		const bool doPosJump = (nPosJump != ORDERINDEX_INVALID);

		if(doPatternLoop)
		{
			m_PlayState.m_nNextOrder = m_PlayState.m_nCurrentOrder;
			m_PlayState.m_nNextRow = nPatLoopRow;
			if(m_PlayState.m_nPatternDelay)
			{
				m_PlayState.m_nNextRow++;
			}

			if(nPatLoopRow >= Patterns[m_PlayState.m_nPattern].GetNumRows())
			{
				m_PlayState.m_nNextOrder++;
				m_PlayState.m_nNextRow = 0;
			}

			visitedSongRows.ResetPatternLoop(m_PlayState.m_nCurrentOrder, nPatLoopRow);
		}

		if((doBreakRow || doPosJump)
			&& (!doPatternLoop || m_playBehaviour[kFT2PatternLoopWithJumps] || (m_playBehaviour[kITPatternLoopWithJumps] && doPosJump)))
		{
			if(!doPosJump) nPosJump = m_PlayState.m_nCurrentOrder + 1;
			if(!doBreakRow) nBreakRow = 0;
			m_SongFlags.set(SONG_BREAKTOROW);

			if(nPosJump >= Order().size())
			{
				nPosJump = Order().GetRestartPos();
			}

			if(nPosJump != m_PlayState.m_nCurrentOrder
				&& !m_playBehaviour[kITPatternLoopBreak] && !m_playBehaviour[kFT2PatternLoopWithJumps] && GetType() != MOD_TYPE_MOD)
			{
				for(CHANNELINDEX i = 0; i < GetNumChannels(); i++)
				{
					m_PlayState.Chn[i].nPatternLoopCount = 0;
				}
			}

			m_PlayState.m_nNextRow = nBreakRow;
			if(!m_SongFlags[SONG_PATTERNLOOP])
				m_PlayState.m_nNextOrder = nPosJump;
		}

	}
	return true;
}
