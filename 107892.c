qboolean S_AL_SrcInit( void )
{
	int i;
	int limit;

	memset(srcList, 0, sizeof(srcList));
	srcCount = 0;
	srcActiveCnt = 0;

	limit = s_alSources->integer;
	if(limit > MAX_SRC)
		limit = MAX_SRC;
	else if(limit < 16)
		limit = 16;
 
	S_AL_ClearError( qfalse );
	for(i = 0; i < limit; i++)
	{
		qalGenSources(1, &srcList[i].alSource);
		if(qalGetError() != AL_NO_ERROR)
			break;
		srcCount++;
	}

	Com_Printf( "Allocated %d sources.\n", srcCount);
	alSourcesInitialised = qtrue;
	return qtrue;
}
