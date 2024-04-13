void CSoundFile::AdjustSampleLoop(MODINSTRUMENT *pIns)
{
	if (!pIns->pSample) return;
	if (pIns->nLoopEnd > pIns->nLength) pIns->nLoopEnd = pIns->nLength;
	if (pIns->nLoopStart+2 >= pIns->nLoopEnd)
	{
		pIns->nLoopStart = pIns->nLoopEnd = 0;
		pIns->uFlags &= ~CHN_LOOP;
	}
	UINT len = pIns->nLength;
	if (pIns->uFlags & CHN_16BIT)
	{
		short int *pSample = (short int *)pIns->pSample;
		if (pIns->uFlags & CHN_STEREO)
		{
			pSample[len*2+6] = pSample[len*2+4] = pSample[len*2+2] = pSample[len*2] = pSample[len*2-2];
			pSample[len*2+7] = pSample[len*2+5] = pSample[len*2+3] = pSample[len*2+1] = pSample[len*2-1];
		} else
		{
			pSample[len+4] = pSample[len+3] = pSample[len+2] = pSample[len+1] = pSample[len] = pSample[len-1];
		}
		if ((pIns->uFlags & (CHN_LOOP|CHN_PINGPONGLOOP|CHN_STEREO)) == CHN_LOOP)
		{
			if ((pIns->nLoopEnd+3 >= pIns->nLength) || (m_nType & MOD_TYPE_S3M))
			{
				pSample[pIns->nLoopEnd] = pSample[pIns->nLoopStart];
				pSample[pIns->nLoopEnd+1] = pSample[pIns->nLoopStart+1];
				pSample[pIns->nLoopEnd+2] = pSample[pIns->nLoopStart+2];
				pSample[pIns->nLoopEnd+3] = pSample[pIns->nLoopStart+3];
				pSample[pIns->nLoopEnd+4] = pSample[pIns->nLoopStart+4];
			}
		}
	} else
	{
		signed char *pSample = pIns->pSample;
#ifndef FASTSOUNDLIB
		if ((pIns->nLength > 0x100) && (m_nType & (MOD_TYPE_MOD|MOD_TYPE_S3M))
		 && (!(pIns->uFlags & CHN_STEREO)))
		{
			int smpend = pSample[pIns->nLength-1], smpfix = 0, kscan;
			for (kscan=pIns->nLength-1; kscan>0; kscan--)
			{
				smpfix = pSample[kscan-1];
				if (smpfix != smpend) break;
			}
			int delta = smpfix - smpend;
			if (((!(pIns->uFlags & CHN_LOOP)) || (kscan > (int)pIns->nLoopEnd))
			 && ((delta < -8) || (delta > 8)))
			{
				while (kscan<(int)pIns->nLength)
				{
					if (!(kscan & 7))
					{
						if (smpfix > 0) smpfix--;
						if (smpfix < 0) smpfix++;
					}
					pSample[kscan] = (signed char)smpfix;
					kscan++;
				}
			}
		}
#endif
		if (pIns->uFlags & CHN_STEREO)
		{
			pSample[len*2+6] = pSample[len*2+4] = pSample[len*2+2] = pSample[len*2] = pSample[len*2-2];
			pSample[len*2+7] = pSample[len*2+5] = pSample[len*2+3] = pSample[len*2+1] = pSample[len*2-1];
		} else
		{
			pSample[len+4] = pSample[len+3] = pSample[len+2] = pSample[len+1] = pSample[len] = pSample[len-1];
		}
		if ((pIns->uFlags & (CHN_LOOP|CHN_PINGPONGLOOP|CHN_STEREO)) == CHN_LOOP)
		{
			if ((pIns->nLoopEnd+3 >= pIns->nLength) || (m_nType & (MOD_TYPE_MOD|MOD_TYPE_S3M)))
			{
				pSample[pIns->nLoopEnd] = pSample[pIns->nLoopStart];
				pSample[pIns->nLoopEnd+1] = pSample[pIns->nLoopStart+1];
				pSample[pIns->nLoopEnd+2] = pSample[pIns->nLoopStart+2];
				pSample[pIns->nLoopEnd+3] = pSample[pIns->nLoopStart+3];
				pSample[pIns->nLoopEnd+4] = pSample[pIns->nLoopStart+4];
			}
		}
	}
}
