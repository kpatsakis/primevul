void S_AL_StartBackgroundTrack( const char *intro, const char *loop )
{
	int i;
	qboolean issame;

	S_AL_StopBackgroundTrack();

	if((!intro || !*intro) && (!loop || !*loop))
		return;

	S_AL_MusicSourceGet();
	if(musicSourceHandle == -1)
		return;

	if (!loop || !*loop)
	{
		loop = intro;
		issame = qtrue;
	}
	else if(intro && *intro && !strcmp(intro, loop))
		issame = qtrue;
	else
		issame = qfalse;

	Q_strncpyz( s_backgroundLoop, loop, sizeof( s_backgroundLoop ) );

	if(!issame)
	{
		intro_stream = S_CodecOpenStream(intro);
	}
	else
		intro_stream = NULL;

	mus_stream = S_CodecOpenStream(s_backgroundLoop);
	if(!mus_stream)
	{
		S_AL_CloseMusicFiles();
		S_AL_MusicSourceFree();
		return;
	}

	if (!S_AL_GenBuffers(NUM_MUSIC_BUFFERS, musicBuffers, "music"))
		return;
	
	for(i = 0; i < NUM_MUSIC_BUFFERS; i++)
	{
		S_AL_MusicProcess(musicBuffers[i]);
	}

	qalSourceQueueBuffers(musicSource, NUM_MUSIC_BUFFERS, musicBuffers);

	S_AL_Gain(musicSource, s_alGain->value * s_musicVolume->value);
	
	qalSourcePlay(musicSource);

	musicPlaying = qtrue;
}
