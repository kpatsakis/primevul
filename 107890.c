srcHandle_t S_AL_SrcAlloc( alSrcPriority_t priority, int entnum, int channel )
{
	int i;
	int empty = -1;
	int weakest = -1;
	int weakest_time = Sys_Milliseconds();
	int weakest_pri = 999;
	float weakest_gain = 1000.0;
	qboolean weakest_isplaying = qtrue;
	int weakest_numloops = 0;
	src_t *curSource;

	for(i = 0; i < srcCount; i++)
	{
		curSource = &srcList[i];
		
		if(curSource->isLocked)
			continue;

		if(!curSource->isActive)
		{
			empty = i;
			break;
		}

		if(curSource->isPlaying)
		{
			if(weakest_isplaying && curSource->priority < priority &&
			   (curSource->priority < weakest_pri ||
			   (!curSource->isLooping && (curSource->scaleGain < weakest_gain || curSource->lastUsedTime < weakest_time))))
			{
				weakest_pri = curSource->priority;
				weakest_time = curSource->lastUsedTime;
				weakest_gain = curSource->scaleGain;
				weakest = i;
			}
		}
		else
		{
			weakest_isplaying = qfalse;
			
			if(weakest < 0 ||
			   knownSfx[curSource->sfx].loopCnt > weakest_numloops ||
			   curSource->priority < weakest_pri ||
			   curSource->lastUsedTime < weakest_time)
			{
				weakest_pri = curSource->priority;
				weakest_time = curSource->lastUsedTime;
				weakest_numloops = knownSfx[curSource->sfx].loopCnt;
				weakest = i;
			}
		}

#if 0
		if((curSource->entity == entnum) && (curSource->channel == channel) && (channel != 0))
		{
			S_AL_SrcKill(i);
			return i;
		}
#endif
	}

	if(empty == -1)
		empty = weakest;
	
	if(empty >= 0)
	{
		S_AL_SrcKill(empty);
		srcList[empty].isActive = qtrue;
		srcActiveCnt++;
	}

	return empty;
}
