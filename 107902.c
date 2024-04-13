void S_AL_StartLocalSound(sfxHandle_t sfx, int channel)
{
	srcHandle_t src;
	
	if(S_AL_CheckInput(0, sfx))
		return;

	src = S_AL_SrcAlloc(SRCPRI_LOCAL, -1, channel);
	
	if(src == -1)
		return;

	S_AL_SrcSetup(src, sfx, SRCPRI_LOCAL, -1, channel, qtrue);

	srcList[src].isPlaying = qtrue;
	qalSourcePlay(srcList[src].alSource);
}
