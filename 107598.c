void CSoundFile::NoteChange(ModChannel *pChn, int note, bool bPorta, bool bResetEnv, bool bManual) const
{
	if (note < NOTE_MIN) return;
	const ModSample *pSmp = pChn->pModSample;
	const ModInstrument *pIns = pChn->pModInstrument;

	const bool newTuning = (GetType() == MOD_TYPE_MPT && pIns != nullptr && pIns->pTuning);
	const int realnote = note;

	if((pIns) && (note - NOTE_MIN < (int)CountOf(pIns->Keyboard)))
	{
		uint32 n = pIns->Keyboard[note - NOTE_MIN];
		if((n) && (n < MAX_SAMPLES))
		{
			pSmp = &Samples[n];
		} else if(m_playBehaviour[kITEmptyNoteMapSlot] && !pChn->HasMIDIOutput())
		{
			return;
		}
		note = pIns->NoteMap[note - NOTE_MIN];
	}
	if(note > NOTE_MAX)
	{
		if(note == NOTE_KEYOFF || !(GetType() & (MOD_TYPE_IT|MOD_TYPE_MPT)))
		{
			KeyOff(pChn);
		} else // Invalid Note -> Note Fade
		{
			if(/*note == NOTE_FADE && */ GetNumInstruments())
				pChn->dwFlags.set(CHN_NOTEFADE);
		}

		if (note == NOTE_NOTECUT)
		{
			pChn->dwFlags.set(CHN_NOTEFADE | CHN_FASTVOLRAMP);
			if ((!(GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT))) || (m_nInstruments != 0 && !m_playBehaviour[kITInstrWithNoteOff])) pChn->nVolume = 0;
			if(m_playBehaviour[kITInstrWithNoteOff]) pChn->increment.Set(0);
			pChn->nFadeOutVol = 0;
		}

		if(m_playBehaviour[kITClearOldNoteAfterCut])
		{
			pChn->nNote = pChn->nNewNote = NOTE_NONE;
		}
		return;
	}

	if(newTuning)
	{
		if(!bPorta || pChn->nNote == NOTE_NONE)
			pChn->nPortamentoDest = 0;
		else
		{
			pChn->nPortamentoDest = pIns->pTuning->GetStepDistance(pChn->nNote, pChn->m_PortamentoFineSteps, static_cast<Tuning::NOTEINDEXTYPE>(note), 0);
			pChn->m_PortamentoFineSteps = -pChn->nPortamentoDest;
		}
	}

	if(!bPorta && (GetType() & (MOD_TYPE_XM | MOD_TYPE_MED | MOD_TYPE_MT2)))
	{
		if(pSmp)
		{
			pChn->nTranspose = pSmp->RelativeTone;
			pChn->nFineTune = pSmp->nFineTune;
		}
	}
	if(!bPorta && pSmp && m_playBehaviour[kITMultiSampleBehaviour])
		pChn->nC5Speed = pSmp->nC5Speed;

	if(bPorta && !pChn->IsSamplePlaying())
	{
		if(m_playBehaviour[kFT2PortaNoNote])
		{
			pChn->nPeriod = 0;
			return;
		} else if(m_playBehaviour[kITPortaNoNote])
		{
			bPorta = false;
		}
	}

	if(GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2|MOD_TYPE_MED|MOD_TYPE_MOD))
	{
		note += pChn->nTranspose;
		Limit(note, NOTE_MIN + 11, NOTE_MIN + 130);	// 119 possible notes
	} else
	{
		Limit(note, NOTE_MIN, NOTE_MAX);
	}
	if(m_playBehaviour[kITRealNoteMapping])
	{
		pChn->nNote = static_cast<ModCommand::NOTE>(Clamp(realnote, NOTE_MIN, NOTE_MAX));
	} else
	{
		pChn->nNote = static_cast<ModCommand::NOTE>(note);
	}
	pChn->m_CalculateFreq = true;

	if ((!bPorta) || (GetType() & (MOD_TYPE_S3M|MOD_TYPE_IT|MOD_TYPE_MPT)))
		pChn->nNewIns = 0;

	uint32 period = GetPeriodFromNote(note, pChn->nFineTune, pChn->nC5Speed);
	pChn->nPanbrelloOffset = 0;

	if(m_playBehaviour[kITPanningReset]) ApplyInstrumentPanning(pChn, pIns, pSmp);

	if(bResetEnv && !bPorta)
	{
		pChn->nVolSwing = pChn->nPanSwing = 0;
		pChn->nResSwing = pChn->nCutSwing = 0;
		if(pIns)
		{
			if(m_playBehaviour[kITNNAReset]) pChn->nNNA = pIns->nNNA;

			if(!pIns->VolEnv.dwFlags[ENV_CARRY]) pChn->VolEnv.Reset();
			if(!pIns->PanEnv.dwFlags[ENV_CARRY]) pChn->PanEnv.Reset();
			if(!pIns->PitchEnv.dwFlags[ENV_CARRY]) pChn->PitchEnv.Reset();

			if(pIns->nVolSwing)
			{
				pChn->nVolSwing = static_cast<int16>(((mpt::random<int8>(AccessPRNG()) * pIns->nVolSwing) / 64 + 1) * (m_playBehaviour[kITSwingBehaviour] ? pChn->nInsVol : ((pChn->nVolume + 1) / 2)) / 199);
			}
			if(pIns->nPanSwing)
			{
				pChn->nPanSwing = static_cast<int16>(((mpt::random<int8>(AccessPRNG()) * pIns->nPanSwing * 4) / 128));
				if(!m_playBehaviour[kITSwingBehaviour])
				{
					pChn->nRestorePanOnNewNote = static_cast<int16>(pChn->nPan + 1);
				}
			}
			if(pIns->nCutSwing)
			{
				int32 d = ((int32)pIns->nCutSwing * (int32)(static_cast<int32>(mpt::random<int8>(AccessPRNG())) + 1)) / 128;
				pChn->nCutSwing = static_cast<int16>((d * pChn->nCutOff + 1) / 128);
				pChn->nRestoreCutoffOnNewNote = pChn->nCutOff + 1;
			}
			if(pIns->nResSwing)
			{
				int32 d = ((int32)pIns->nResSwing * (int32)(static_cast<int32>(mpt::random<int8>(AccessPRNG())) + 1)) / 128;
				pChn->nResSwing = static_cast<int16>((d * pChn->nResonance + 1) / 128);
				pChn->nRestoreResonanceOnNewNote = pChn->nResonance + 1;
			}
		}
	}

	if(!pSmp) return;
	if(period)
	{
		if((!bPorta) || (!pChn->nPeriod)) pChn->nPeriod = period;
		if(!newTuning)
		{
			if(bPorta || !(m_playBehaviour[kFT2PortaTargetNoReset] || m_playBehaviour[kITClearPortaTarget] || GetType() == MOD_TYPE_MOD))
			{
				pChn->nPortamentoDest = period;
			}
		}

		if(!bPorta || (!pChn->nLength && !(GetType() & MOD_TYPE_S3M)))
		{
			pChn->pModSample = pSmp;
			pChn->nLength = pSmp->nLength;
			pChn->nLoopEnd = pSmp->nLength;
			pChn->nLoopStart = 0;
			pChn->position.Set(0);
			if(m_SongFlags[SONG_PT_MODE] && !pChn->rowCommand.instr)
			{
				pChn->position.SetInt(std::min<SmpLength>(pChn->proTrackerOffset, pChn->nLength - 1));
			} else
			{
				pChn->proTrackerOffset = 0;
			}
			pChn->dwFlags = (pChn->dwFlags & CHN_CHANNELFLAGS) | (pSmp->uFlags & (CHN_SAMPLEFLAGS | CHN_SURROUND));
			pChn->dwFlags.reset(CHN_PORTAMENTO);
			if(pChn->dwFlags[CHN_SUSTAINLOOP])
			{
				pChn->nLoopStart = pSmp->nSustainStart;
				pChn->nLoopEnd = pSmp->nSustainEnd;
				pChn->dwFlags.set(CHN_PINGPONGLOOP, pChn->dwFlags[CHN_PINGPONGSUSTAIN]);
				pChn->dwFlags.set(CHN_LOOP);
				if (pChn->nLength > pChn->nLoopEnd) pChn->nLength = pChn->nLoopEnd;
			} else if(pChn->dwFlags[CHN_LOOP])
			{
				pChn->nLoopStart = pSmp->nLoopStart;
				pChn->nLoopEnd = pSmp->nLoopEnd;
				if (pChn->nLength > pChn->nLoopEnd) pChn->nLength = pChn->nLoopEnd;
			}
			if(m_playBehaviour[kMODOneShotLoops] && pChn->nLoopStart == 0) pChn->nLoopEnd = pChn->nLength = pSmp->nLength;

			if(pChn->dwFlags[CHN_REVERSE])
			{
				pChn->dwFlags.set(CHN_PINGPONGFLAG);
				pChn->position.SetInt(pChn->nLength - 1);
			}

			if(pChn->nVibratoType < 4)
			{
				if(!m_playBehaviour[kITVibratoTremoloPanbrello] && (GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT)) && !m_SongFlags[SONG_ITOLDEFFECTS])
					pChn->nVibratoPos = 0x10;
				else if(GetType() == MOD_TYPE_MTM)
					pChn->nVibratoPos = 0x20;
				else if(!(GetType() & (MOD_TYPE_DIGI | MOD_TYPE_DBM)))
					pChn->nVibratoPos = 0;
			}
			if(!m_playBehaviour[kITVibratoTremoloPanbrello] && pChn->nTremoloType < 4)
			{
				pChn->nTremoloPos = 0;
			}
		}
		if(pChn->position.GetUInt() >= pChn->nLength) pChn->position.SetInt(pChn->nLoopStart);
	} else
	{
		bPorta = false;
	}

	if (!bPorta
		|| (!(GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT | MOD_TYPE_DBM)))
		|| (pChn->dwFlags[CHN_NOTEFADE] && !pChn->nFadeOutVol)
		|| (m_SongFlags[SONG_ITCOMPATGXX] && pChn->rowCommand.instr != 0))
	{
		if((GetType() & (MOD_TYPE_IT | MOD_TYPE_MPT | MOD_TYPE_DBM)) && pChn->dwFlags[CHN_NOTEFADE] && !pChn->nFadeOutVol)
		{
			pChn->ResetEnvelopes();
			if(!m_playBehaviour[kITVibratoTremoloPanbrello])
			{
				pChn->nAutoVibDepth = 0;
				pChn->nAutoVibPos = 0;
			}
			pChn->dwFlags.reset(CHN_NOTEFADE);
			pChn->nFadeOutVol = 65536;
		}
		if ((!bPorta) || (!m_SongFlags[SONG_ITCOMPATGXX]) || (pChn->rowCommand.instr))
		{
			if ((!(GetType() & (MOD_TYPE_XM|MOD_TYPE_MT2))) || (pChn->rowCommand.instr))
			{
				pChn->dwFlags.reset(CHN_NOTEFADE);
				pChn->nFadeOutVol = 65536;
			}
		}
	}

	if(m_playBehaviour[kITDontResetNoteOffOnPorta] && bPorta && (!m_SongFlags[SONG_ITCOMPATGXX] || pChn->rowCommand.instr == 0))
		pChn->dwFlags.reset(CHN_EXTRALOUD);
	else
		pChn->dwFlags.reset(CHN_EXTRALOUD | CHN_KEYOFF);

	if(!bPorta)
	{
		pChn->nLeftVU = pChn->nRightVU = 0xFF;
		pChn->dwFlags.reset(CHN_FILTER);
		pChn->dwFlags.set(CHN_FASTVOLRAMP);

		if(!m_playBehaviour[kITRetrigger] && !m_playBehaviour[kITTremor])
		{
			if(!m_playBehaviour[kFT2Retrigger] && !m_playBehaviour[kFT2Tremor])
			{
				pChn->nRetrigCount = 0;
				pChn->nTremorCount = 0;
			}
		}

		if(bResetEnv)
		{
			pChn->nAutoVibDepth = 0;
			pChn->nAutoVibPos = 0;
		}
		pChn->rightVol = pChn->leftVol = 0;
		bool useFilter = !m_SongFlags[SONG_MPTFILTERMODE];
		if(pIns)
		{
			if(pIns->IsResonanceEnabled())
			{
				pChn->nResonance = pIns->GetResonance();
				useFilter = true;
			}
			if(pIns->IsCutoffEnabled())
			{
				pChn->nCutOff = pIns->GetCutoff();
				useFilter = true;
			}
			if(useFilter && (pIns->nFilterMode != FLTMODE_UNCHANGED))
			{
				pChn->nFilterMode = pIns->nFilterMode;
			}
		} else
		{
			pChn->nVolSwing = pChn->nPanSwing = 0;
			pChn->nCutSwing = pChn->nResSwing = 0;
		}
		if((pChn->nCutOff < 0x7F || m_playBehaviour[kITFilterBehaviour]) && useFilter)
		{
			SetupChannelFilter(pChn, true);
		}
	}
	if (bManual) pChn->dwFlags.reset(CHN_MUTE);
	if((pChn->dwFlags[CHN_MUTE] && (m_MixerSettings.MixerFlags & SNDMIX_MUTECHNMODE))
		|| (pChn->pModSample != nullptr && pChn->pModSample->uFlags[CHN_MUTE] && !bManual)
		|| (pChn->pModInstrument != nullptr && pChn->pModInstrument->dwFlags[INS_MUTE] && !bManual))
	{
		if (!bManual) pChn->nPeriod = 0;
	}

	if(!bPorta)
	{
		pChn->paulaState.Reset();
	}
}
