qboolean S_AL_BufferInit( void )
{
	if(alBuffersInitialised)
		return qtrue;

	memset(knownSfx, 0, sizeof(knownSfx));
	numSfx = 0;

	default_sfx = S_AL_BufferFind("sound/feedback/hit.wav");
	S_AL_BufferUse(default_sfx);
	knownSfx[default_sfx].isLocked = qtrue;

	alBuffersInitialised = qtrue;
	return qtrue;
}
