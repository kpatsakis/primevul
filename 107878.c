static void S_AL_MusicSourceFree( void )
{
	S_AL_SrcUnlock(musicSourceHandle);
	S_AL_SrcKill(musicSourceHandle);
	musicSource = 0;
	musicSourceHandle = -1;
}
