void CSoundFile::ProcessMIDIMacro(CHANNELINDEX nChn, bool isSmooth, const char *macro, uint8 param, PLUGINDEX plugin)
{
	ModChannel &chn = m_PlayState.Chn[nChn];
	const ModInstrument *pIns = GetNumInstruments() ? chn.pModInstrument : nullptr;

	uint8 out[MACRO_LENGTH];
	uint32 outPos = 0;	// output buffer position, which also equals the number of complete bytes
	const uint8 lastZxxParam = chn.lastZxxParam;
	bool firstNibble = true;

	for(uint32 pos = 0; pos < (MACRO_LENGTH - 1) && macro[pos]; pos++)
	{
		bool isNibble = false;		// did we parse a nibble or a byte value?
		uint8 data = 0;		// data that has just been parsed

		if(macro[pos] >= '0' && macro[pos] <= '9')
		{
			isNibble = true;
			data = static_cast<uint8>(macro[pos] - '0');
		}
		else if(macro[pos] >= 'A' && macro[pos] <= 'F')
		{
			isNibble = true;
			data = static_cast<uint8>(macro[pos] - 'A' + 0x0A);
		} else if(macro[pos] == 'c')
		{
			isNibble = true;
			data = GetBestMidiChannel(nChn);
		} else if(macro[pos] == 'n')
		{
			if(ModCommand::IsNote(chn.nLastNote))
			{
				data = chn.nLastNote - NOTE_MIN;
			}
		} else if(macro[pos] == 'v')
		{
			const int swing = (m_playBehaviour[kITSwingBehaviour] || m_playBehaviour[kMPTOldSwingBehaviour]) ? chn.nVolSwing : 0;
			const int vol = Util::muldiv((chn.nVolume + swing) * m_PlayState.m_nGlobalVolume, chn.nGlobalVol * chn.nInsVol, 1 << 20);
			data = static_cast<uint8>(Clamp(vol / 2, 1, 127));
		} else if(macro[pos] == 'u')
		{
			const int vol = Util::muldiv(chn.nCalcVolume * m_PlayState.m_nGlobalVolume, chn.nGlobalVol * chn.nInsVol, 1 << 26);
			data = static_cast<uint8>(Clamp(vol / 2, 1, 127));
		} else if(macro[pos] == 'x')
		{
			data = static_cast<uint8>(std::min(chn.nPan / 2, 127));
		} else if(macro[pos] == 'y')
		{
			data = static_cast<uint8>(std::min(chn.nRealPan / 2, 127));
		} else if(macro[pos] == 'a')
		{
			if(pIns && pIns->wMidiBank)
			{
				data = static_cast<uint8>(((pIns->wMidiBank - 1) >> 7) & 0x7F);
			}
		} else if(macro[pos] == 'b')
		{
			if(pIns && pIns->wMidiBank)
			{
				data = static_cast<uint8>((pIns->wMidiBank - 1) & 0x7F);
			}
		} else if(macro[pos] == 'o')
		{
			data = static_cast<uint8>((chn.oldOffset >> 8) & 0xFF);
		} else if(macro[pos] == 'h')
		{
			data = static_cast<uint8>((nChn >= GetNumChannels() ? (chn.nMasterChn - 1) : nChn) & 0x7F);
		} else if(macro[pos] == 'm')
		{
			data = chn.dwFlags[CHN_PINGPONGFLAG] ? 1 : 0;
		} else if(macro[pos] == 'p')
		{
			if(pIns && pIns->nMidiProgram)
			{
				data = static_cast<uint8>((pIns->nMidiProgram - 1) & 0x7F);
			}
		} else if(macro[pos] == 'z')
		{
			data = param & 0x7F;
			if(isSmooth && chn.lastZxxParam < 0x80
				&& (outPos < 3 || out[outPos - 3] != 0xF0 || out[outPos - 2] < 0xF0))
			{
				data = static_cast<uint8>(CalculateSmoothParamChange((float)lastZxxParam, (float)data));
			}
			chn.lastZxxParam = data;
		} else if(macro[pos] == 's')
		{
			uint32 startPos = outPos;
			while(startPos > 0 && out[--startPos] != 0xF0);
			if(outPos - startPos < 5 || out[startPos] != 0xF0)
			{
				continue;
			}
			for(uint32 p = startPos + 5; p != outPos; p++)
			{
				data += out[p];
			}
			data = (~data + 1) & 0x7F;
		} else
		{
			continue;
		}

		if(isNibble)	// parsed a nibble (constant or 'c' variable)
		{
			if(firstNibble)
			{
				out[outPos] = data;
			} else
			{
				out[outPos] = (out[outPos] << 4) | data;
				outPos++;
			}
			firstNibble = !firstNibble;
		} else			// parsed a byte (variable)
		{
			if(!firstNibble)	// From MIDI.TXT: '9n' is exactly the same as '09 n' or '9 n' -- so finish current byte first
			{
				outPos++;
			}
			out[outPos++] = data;
			firstNibble = true;
		}
	}
	if(!firstNibble)
	{
		outPos++;
	}

	uint32 sendPos = 0;
	uint8 runningStatus = 0;
	while(sendPos < outPos)
	{
		uint32 sendLen = 0;
		if(out[sendPos] == 0xF0)
		{
			if((outPos - sendPos >= 4) && (out[sendPos + 1] == 0xF0 || out[sendPos + 1] == 0xF1))
			{
				sendLen = 4;
			} else
			{
				for(uint32 i = sendPos + 1; i < outPos; i++)
				{
					if(out[i] == 0xF7)
					{
						sendLen = i - sendPos + 1;
						break;
					}
				}
				if(sendLen == 0)
				{
					out[outPos++] = 0xF7;
					sendLen = outPos - sendPos;
				}
			}
		} else if(!(out[sendPos] & 0x80))
		{
			if(runningStatus != 0)
			{
				sendPos--;
				out[sendPos] = runningStatus;
			} else
			{
				sendPos++;
			}
			continue;
		} else
		{
			sendLen = std::min<uint32>(MIDIEvents::GetEventLength(out[sendPos]), outPos - sendPos);
		}

		if(sendLen == 0)
		{
			break;
		}

		if(out[sendPos] < 0xF0)
		{
			runningStatus = out[sendPos];
		}
		uint32 bytesSent = SendMIDIData(nChn, isSmooth, out + sendPos, sendLen, plugin);
		if(bytesSent > 0)
		{
			sendPos += bytesSent;
		} else
		{
			sendPos += sendLen;
		}
	}
}
