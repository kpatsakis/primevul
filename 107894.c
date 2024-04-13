static void S_AL_SrcLoop( alSrcPriority_t priority, sfxHandle_t sfx,
		const vec3_t origin, const vec3_t velocity, int entityNum )
{
	int				src;
	sentity_t	*sent = &entityList[ entityNum ];
	src_t		*curSource;
	vec3_t		sorigin, svelocity;

	if(S_AL_CheckInput(entityNum, sfx))
		return;

	if( !sent->srcAllocated )
	{
		src = S_AL_SrcAlloc( priority, entityNum, -1 );
		if( src == -1 )
		{
			Com_DPrintf( S_COLOR_YELLOW "WARNING: Failed to allocate source "
					"for loop sfx %d on entity %d\n", sfx, entityNum );
			return;
		}

		curSource = &srcList[src];

		sent->startLoopingSound = qtrue;

		curSource->lastTimePos = -1.0;
		curSource->lastSampleTime = Sys_Milliseconds();
	}
	else
	{
		src = sent->srcIndex;
		curSource = &srcList[src];
	}

	sent->srcAllocated = qtrue;
	sent->srcIndex = src;

	sent->loopPriority = priority;
	sent->loopSfx = sfx;

	sent->loopAddedThisFrame = qtrue;

	curSource->entity = entityNum;
	curSource->isLooping = qtrue;

	if( S_AL_HearingThroughEntity( entityNum ) )
	{
		curSource->local = qtrue;

		VectorClear(sorigin);

		qalSourcefv(curSource->alSource, AL_POSITION, sorigin);
		qalSourcefv(curSource->alSource, AL_VELOCITY, vec3_origin);
	}
	else
	{
		curSource->local = qfalse;

		if(origin)
			VectorCopy(origin, sorigin);
		else
			VectorCopy(sent->origin, sorigin);

		S_AL_SanitiseVector(sorigin);
		
		VectorCopy(sorigin, curSource->loopSpeakerPos);
		
		if(velocity)
		{
			VectorCopy(velocity, svelocity);
			S_AL_SanitiseVector(svelocity);
		}
		else
			VectorClear(svelocity);

		qalSourcefv(curSource->alSource, AL_POSITION, (ALfloat *) sorigin);
		qalSourcefv(curSource->alSource, AL_VELOCITY, (ALfloat *) svelocity);
	}
}
