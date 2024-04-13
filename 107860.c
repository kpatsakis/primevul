void S_AL_BufferShutdown( void )
{
	int i;

	if(!alBuffersInitialised)
		return;

	knownSfx[default_sfx].isLocked = qfalse;

	for(i = 0; i < numSfx; i++)
		S_AL_BufferUnload(i);

	numSfx = 0;

	alBuffersInitialised = qfalse;
}
