static void S_AL_AllocateStreamChannel(int stream, int entityNum)
{
        srcHandle_t cursrc;
        ALuint alsrc;
        
	if ((stream < 0) || (stream >= MAX_RAW_STREAMS))
		return;

        if(entityNum >= 0)
        {
        	cursrc = S_AL_SrcAlloc(SRCPRI_ENTITY, entityNum, 0);
        	if(cursrc < 0)
	        	return;

        	S_AL_SrcSetup(cursrc, -1, SRCPRI_ENTITY, entityNum, 0, qfalse);
        	alsrc = S_AL_SrcGet(cursrc);
        	srcList[cursrc].isTracking = qtrue;
        	srcList[cursrc].isStream = qtrue;
        }
        else
        {

        	cursrc = S_AL_SrcAlloc(SRCPRI_STREAM, -2, 0);
        	if(cursrc < 0)
	        	return;

        	alsrc = S_AL_SrcGet(cursrc);

        	S_AL_SrcLock(cursrc);
        
        	srcList[cursrc].scaleGain = 0.0f;

        	qalSourcei (alsrc, AL_BUFFER,          0            );
        	qalSourcei (alsrc, AL_LOOPING,         AL_FALSE     );
        	qalSource3f(alsrc, AL_POSITION,        0.0, 0.0, 0.0);
        	qalSource3f(alsrc, AL_VELOCITY,        0.0, 0.0, 0.0);
        	qalSource3f(alsrc, AL_DIRECTION,       0.0, 0.0, 0.0);
        	qalSourcef (alsrc, AL_ROLLOFF_FACTOR,  0.0          );
        	qalSourcei (alsrc, AL_SOURCE_RELATIVE, AL_TRUE      );
        }

        streamSourceHandles[stream] = cursrc;
       	streamSources[stream] = alsrc;

	streamNumBuffers[stream] = 0;
	streamBufIndex[stream] = 0;
}
