void S_AL_SrcUpdate( void )
{
	int i;
	int entityNum;
	ALint state;
	src_t *curSource;
	
	for(i = 0; i < srcCount; i++)
	{
		entityNum = srcList[i].entity;
		curSource = &srcList[i];

		if(curSource->isLocked)
			continue;

		if(!curSource->isActive)
			continue;

		if((s_alGain->modified) || (s_volume->modified))
			curSource->curGain = s_alGain->value * s_volume->value;
		if((s_alRolloff->modified) && (!curSource->local))
			qalSourcef(curSource->alSource, AL_ROLLOFF_FACTOR, s_alRolloff->value);
		if(s_alMinDistance->modified)
			qalSourcef(curSource->alSource, AL_REFERENCE_DISTANCE, s_alMinDistance->value);

		if(curSource->isLooping)
		{
			sentity_t *sent = &entityList[ entityNum ];

			if(sent->loopAddedThisFrame)
			{
				alSfx_t *curSfx;
			
				if(curSource->isActive && !sent->startLoopingSound &&
						curSource->sfx != sent->loopSfx)
				{
					S_AL_NewLoopMaster(curSource, qtrue);

					curSource->isPlaying = qfalse;
					qalSourceStop(curSource->alSource);
					qalSourcei(curSource->alSource, AL_BUFFER, 0);
					sent->startLoopingSound = qtrue;
				}

				if(sent->startLoopingSound)
				{
					S_AL_SrcSetup(i, sent->loopSfx, sent->loopPriority,
							entityNum, -1, curSource->local);
					curSource->isLooping = qtrue;
					
					knownSfx[curSource->sfx].loopCnt++;
					sent->startLoopingSound = qfalse;
				}
				
				curSfx = &knownSfx[curSource->sfx];

				S_AL_ScaleGain(curSource, curSource->loopSpeakerPos);
				if(!curSource->scaleGain)
				{
					if(curSource->isPlaying)
					{
						S_AL_NewLoopMaster(curSource, qfalse);
						qalSourceStop(curSource->alSource);
						curSource->isPlaying = qfalse;
					}
					else if(!curSfx->loopActiveCnt && curSfx->masterLoopSrc < 0)
						curSfx->masterLoopSrc = i;
					
					continue;
				}

				if(!curSource->isPlaying)
				{
					qalSourcei(curSource->alSource, AL_LOOPING, AL_TRUE);
					curSource->isPlaying = qtrue;
					qalSourcePlay(curSource->alSource);

					if(curSource->priority == SRCPRI_AMBIENT)
					{

						if(curSfx->loopActiveCnt)
						{
							int offset, error;
						
							S_AL_ClearError(qfalse);
							qalGetSourcei(srcList[curSfx->masterLoopSrc].alSource, AL_SAMPLE_OFFSET, &offset);
							if((error = qalGetError()) != AL_NO_ERROR)
							{
								if(error != AL_INVALID_ENUM)
								{
									Com_Printf(S_COLOR_YELLOW "WARNING: Cannot get sample offset from source %d: "
										   "%s\n", i, S_AL_ErrorMsg(error));
								}
							}
							else
								qalSourcei(curSource->alSource, AL_SAMPLE_OFFSET, offset);
						}
						else if(curSfx->loopCnt && curSfx->masterLoopSrc >= 0)
						{
							float secofs;
						
							src_t *master = &srcList[curSfx->masterLoopSrc];
						
							if(master->lastTimePos >= 0)
							{
								secofs = master->lastTimePos + (Sys_Milliseconds() - master->lastSampleTime) / 1000.0f;
								secofs = fmodf(secofs, (float) curSfx->info.samples / curSfx->info.rate);
						
								qalSourcef(curSource->alSource, AL_SEC_OFFSET, secofs);
							}

							curSfx->masterLoopSrc = i;
						}
						else
							curSfx->masterLoopSrc = i;
					}
					else if(curSource->lastTimePos >= 0)
					{
						float secofs;
						
						
						secofs = curSource->lastTimePos + (Sys_Milliseconds() - curSource->lastSampleTime) / 1000.0f;
						secofs = fmodf(secofs, (float) curSfx->info.samples / curSfx->info.rate);
						qalSourcef(curSource->alSource, AL_SEC_OFFSET, secofs);
					}
						
					curSfx->loopActiveCnt++;
				}

				if(curSource->local)
				{
					qalSourcei(curSource->alSource, AL_SOURCE_RELATIVE, AL_TRUE);
					qalSourcef(curSource->alSource, AL_ROLLOFF_FACTOR, 0.0f);
				}
				else
				{
					qalSourcei(curSource->alSource, AL_SOURCE_RELATIVE, AL_FALSE);
					qalSourcef(curSource->alSource, AL_ROLLOFF_FACTOR, s_alRolloff->value);
				}
				
			}
			else if(curSource->priority == SRCPRI_AMBIENT)
			{
				if(curSource->isPlaying)
				{
					S_AL_NewLoopMaster(curSource, qfalse);
					qalSourceStop(curSource->alSource);
					curSource->isPlaying = qfalse;
				}
			}
			else
				S_AL_SrcKill(i);

			continue;
		}

		if(!curSource->isStream)
		{
	        	qalGetSourcei(curSource->alSource, AL_SOURCE_STATE, &state);
	        	if(state == AL_STOPPED)
        		{
	        		curSource->isPlaying = qfalse;
		        	S_AL_SrcKill(i);
		        	continue;
        		}
                }

		qalGetSourcei(curSource->alSource, AL_SOURCE_RELATIVE, &state);

		if(curSource->isTracking && !state)
		{
			qalSourcefv(curSource->alSource, AL_POSITION, entityList[entityNum].origin);
 			S_AL_ScaleGain(curSource, entityList[entityNum].origin);
		}
	}
}
