void S_AL_StopBackgroundTrack( void )
{
	if(!musicPlaying)
		return;

	qalSourceStop(musicSource);

	qalSourcei(musicSource, AL_BUFFER, 0);

	qalDeleteBuffers(NUM_MUSIC_BUFFERS, musicBuffers);

	S_AL_MusicSourceFree();

	S_AL_CloseMusicFiles();

	musicPlaying = qfalse;
}
