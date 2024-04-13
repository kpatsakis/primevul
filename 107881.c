static void S_AL_NewLoopMaster(src_t *rmSource, qboolean iskilled)
{
	int index;
	src_t *curSource = NULL;
	alSfx_t *curSfx;
	
	curSfx = &knownSfx[rmSource->sfx];

	if(rmSource->isPlaying)
		curSfx->loopActiveCnt--;
	if(iskilled)
		curSfx->loopCnt--;
	
	if(curSfx->loopCnt)
	{
		if(rmSource->priority == SRCPRI_ENTITY)
		{
			if(!iskilled && rmSource->isPlaying)
			{
				S_AL_SaveLoopPos(rmSource, rmSource->alSource);
			}
		}
		else if(rmSource == &srcList[curSfx->masterLoopSrc])
		{
			int firstInactive = -1;

	
			if(iskilled || curSfx->loopActiveCnt)
			{
				for(index = 0; index < srcCount; index++)
				{
					curSource = &srcList[index];
	
					if(curSource->sfx == rmSource->sfx && curSource != rmSource &&
					   curSource->isActive && curSource->isLooping && curSource->priority == SRCPRI_AMBIENT)
					{
						if(curSource->isPlaying)
						{
							curSfx->masterLoopSrc = index;
							break;
						}
						else if(firstInactive < 0)
							firstInactive = index;
					}
				}
			}
		
			if(!curSfx->loopActiveCnt)
			{
				if(firstInactive < 0)
				{
					if(iskilled)
					{
						curSfx->masterLoopSrc = -1;
						return;
					}
					else
						curSource = rmSource;
				}
				else
					curSource = &srcList[firstInactive];

				if(rmSource->isPlaying)
				{
					S_AL_SaveLoopPos(curSource, rmSource->alSource);
				}
				else
				{
					curSource->lastTimePos = rmSource->lastTimePos;
					curSource->lastSampleTime = rmSource->lastSampleTime;
				}
			}
		}
	}
	else
		curSfx->masterLoopSrc = -1;
}
