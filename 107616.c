void CSoundFile::SampleOffset(ModChannel &chn, SmpLength param) const
{
	chn.proTrackerOffset += param;

	if(param >= chn.nLoopEnd && GetType() == MOD_TYPE_MTM && chn.dwFlags[CHN_LOOP] && chn.nLoopEnd > 0)
	{
		param = (param - chn.nLoopStart) % (chn.nLoopEnd - chn.nLoopStart) + chn.nLoopStart;
	}

	if(GetType() == MOD_TYPE_MDL && chn.dwFlags[CHN_16BIT])
	{
		param /= 2u;
	}

	if(chn.rowCommand.IsNote())
	{
		if(chn.pModInstrument != nullptr)
		{
			SAMPLEINDEX smp = chn.pModInstrument->Keyboard[chn.rowCommand.note - NOTE_MIN];
			if(smp == 0 || smp > GetNumSamples())
				return;
		}

		if(m_SongFlags[SONG_PT_MODE])
		{
			chn.position.Set(chn.proTrackerOffset);
			chn.proTrackerOffset += param;
		} else
		{
			chn.position.Set(param);
		}

		if (chn.position.GetUInt() >= chn.nLength || (chn.dwFlags[CHN_LOOP] && chn.position.GetUInt() >= chn.nLoopEnd))
		{
			if (!(GetType() & (MOD_TYPE_XM | MOD_TYPE_MT2 | MOD_TYPE_MOD | MOD_TYPE_MTM)))
			{
				if(m_playBehaviour[kITOffset])
				{
					if(m_SongFlags[SONG_ITOLDEFFECTS])
						chn.position.Set(chn.nLength); // Old FX: Clip to end of sample
					else
						chn.position.Set(0); // Reset to beginning of sample
				} else
				{
					chn.position.Set(chn.nLoopStart);
					if(m_SongFlags[SONG_ITOLDEFFECTS] && chn.nLength > 4)
					{
						chn.position.Set(chn.nLength - 2);
					}
				}
			} else if(m_playBehaviour[kFT2OffsetOutOfRange] || GetType() == MOD_TYPE_MTM)
			{
				chn.dwFlags.set(CHN_FASTVOLRAMP);
				chn.nPeriod = 0;
			} else if(GetType() == MOD_TYPE_MOD && chn.dwFlags[CHN_LOOP])
			{
				chn.position.Set(chn.nLoopStart);
			}
		}
	} else if ((param < chn.nLength) && (GetType() & (MOD_TYPE_MTM | MOD_TYPE_DMF | MOD_TYPE_MDL | MOD_TYPE_PLM)))
	{
		chn.position.Set(param);
	}
}
