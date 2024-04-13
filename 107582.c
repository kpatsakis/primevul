IMixPlugin *CSoundFile::GetChannelInstrumentPlugin(CHANNELINDEX chn) const
{
#ifndef NO_PLUGINS
	if(m_PlayState.Chn[chn].dwFlags[CHN_MUTE | CHN_SYNCMUTE])
	{
		return nullptr;
	}

	if(m_PlayState.Chn[chn].HasMIDIOutput())
	{
		const ModInstrument *pIns = m_PlayState.Chn[chn].pModInstrument;
		if(pIns->nMixPlug != 0 && pIns->nMixPlug <= MAX_MIXPLUGINS)
		{
			return m_MixPlugins[pIns->nMixPlug - 1].pMixPlugin;
		}
	}
#else
	MPT_UNREFERENCED_PARAMETER(chn);
#endif // NO_PLUGINS
	return nullptr;
}
