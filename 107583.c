PLUGINDEX CSoundFile::GetChannelPlugin(CHANNELINDEX nChn, PluginMutePriority respectMutes) const
{
	const ModChannel &channel = m_PlayState.Chn[nChn];

	PLUGINDEX nPlugin;
	if((respectMutes == RespectMutes && channel.dwFlags[CHN_MUTE]) || channel.dwFlags[CHN_NOFX])
	{
		nPlugin = 0;
	} else
	{
		if (nChn >= m_nChannels && channel.nMasterChn > 0)
		{
			nChn = channel.nMasterChn - 1;
		}

		if(nChn < MAX_BASECHANNELS)
		{
			nPlugin = ChnSettings[nChn].nMixPlugin;
		} else
		{
			nPlugin = 0;
		}
	}
	return nPlugin;
}
