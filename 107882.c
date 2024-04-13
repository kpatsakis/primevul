void S_AL_RawSamples(int stream, int samples, int rate, int width, int channels, const byte *data, float volume, int entityNum)
{
	int numBuffers;
	ALuint buffer;
	ALuint format;

	if ((stream < 0) || (stream >= MAX_RAW_STREAMS))
		return;

	format = S_AL_Format( width, channels );

	if(streamSourceHandles[stream] == -1)
	{
		S_AL_AllocateStreamChannel(stream, entityNum);
	
		if(streamSourceHandles[stream] == -1)
		{
			Com_Printf( S_COLOR_RED "ERROR: Can't allocate streaming streamSource\n");
			return;
		}
	}

	qalGetSourcei(streamSources[stream], AL_BUFFERS_QUEUED, &numBuffers);

	if (numBuffers == MAX_STREAM_BUFFERS)
	{
		Com_DPrintf(S_COLOR_RED"WARNING: Steam dropping raw samples, reached MAX_STREAM_BUFFERS\n");
		return;
	}

	if (numBuffers == streamNumBuffers[stream])
	{
		ALuint oldBuffers[MAX_STREAM_BUFFERS];
		int	i;

		if (!S_AL_GenBuffers(1, &buffer, "stream"))
			return;

		Com_Memcpy(oldBuffers, &streamBuffers[stream], sizeof (oldBuffers));

		for ( i = 0; i < streamNumBuffers[stream]; ++i )
			streamBuffers[stream][i] = oldBuffers[(streamBufIndex[stream] + i) % streamNumBuffers[stream]];

		streamBuffers[stream][streamNumBuffers[stream]] = buffer;
		streamBufIndex[stream] = streamNumBuffers[stream];
		streamNumBuffers[stream]++;
	}

	buffer = streamBuffers[stream][ streamBufIndex[stream] ];
	streamBufIndex[stream] = (streamBufIndex[stream] + 1) % streamNumBuffers[stream];

	qalBufferData(buffer, format, (ALvoid *)data, (samples * width * channels), rate);

	qalSourceQueueBuffers(streamSources[stream], 1, &buffer);

	if(entityNum < 0)
	{
        	S_AL_Gain (streamSources[stream], volume * s_volume->value * s_alGain->value);
        }

	if(!streamPlaying[stream])
	{
		qalSourcePlay( streamSources[stream] );
		streamPlaying[stream] = qtrue;
	}
}
