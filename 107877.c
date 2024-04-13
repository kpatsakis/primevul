void S_AL_MusicProcess(ALuint b)
{
	ALenum error;
	int l;
	ALuint format;
	snd_stream_t *curstream;

	S_AL_ClearError( qfalse );

	if(intro_stream)
		curstream = intro_stream;
	else
		curstream = mus_stream;

	if(!curstream)
		return;

	l = S_CodecReadStream(curstream, MUSIC_BUFFER_SIZE, decode_buffer);

	if(l == 0)
	{
		S_CodecCloseStream(curstream);

		if(intro_stream)
			intro_stream = NULL;
		else
			mus_stream = S_CodecOpenStream(s_backgroundLoop);
		
		curstream = mus_stream;

		if(!curstream)
		{
			S_AL_StopBackgroundTrack();
			return;
		}

		l = S_CodecReadStream(curstream, MUSIC_BUFFER_SIZE, decode_buffer);
	}

	format = S_AL_Format(curstream->info.width, curstream->info.channels);

	if( l == 0 )
	{
		byte dummyData[ 2 ] = { 0 };

		qalBufferData( b, AL_FORMAT_MONO16, (void *)dummyData, 2, 22050 );
	}
	else
		qalBufferData(b, format, decode_buffer, l, curstream->info.rate);

	if( ( error = qalGetError( ) ) != AL_NO_ERROR )
	{
		S_AL_StopBackgroundTrack( );
		Com_Printf( S_COLOR_RED "ERROR: while buffering data for music stream - %s\n",
				S_AL_ErrorMsg( error ) );
		return;
	}
}
