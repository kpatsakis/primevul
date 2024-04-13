UINT CSoundFile::WriteSample(FILE *f, MODINSTRUMENT *pins, UINT nFlags, UINT nMaxLen)
{
	UINT len = 0, bufcount;
	signed char buffer[4096];
	signed char *pSample = (signed char *)pins->pSample;
	UINT nLen = pins->nLength;

	if ((nMaxLen) && (nLen > nMaxLen)) nLen = nMaxLen;
	if ((!pSample) || (f == NULL) || (!nLen)) return 0;
	switch(nFlags)
	{
#ifndef NO_PACKING
	case RS_ADPCM4:
		{
			int pos;
			len = (nLen + 1) / 2;
			fwrite(CompressionTable, 16, 1, f);
			bufcount = 0;
			pos = 0;
			for (UINT j=0; j<len; j++)
			{
				BYTE b;
				b = PackSample(pos, (int)pSample[j*2]);
				b |= PackSample(pos, (int)pSample[j*2+1]) << 4;
				buffer[bufcount++] = (signed char)b;
				if (bufcount >= sizeof(buffer))
				{
					fwrite(buffer, 1, bufcount, f);
					bufcount = 0;
				}
			}
			if (bufcount) fwrite(buffer, 1, bufcount, f);
			len += 16;
		}
		break;
#endif // NO_PACKING

	case RS_PCM16U:
	case RS_PCM16D:
	case RS_PCM16S:
		{
			short int *p = (short int *)pSample;
			int s_old = 0, s_ofs;
			len = nLen * 2;
			bufcount = 0;
			s_ofs = (nFlags == RS_PCM16U) ? 0x8000 : 0;
			for (UINT j=0; j<nLen; j++)
			{
				int s_new = *p;
				p++;
				if (pins->uFlags & CHN_STEREO)
				{
					s_new = (s_new + (*p) + 1) >> 1;
					p++;
				}
				if (nFlags == RS_PCM16D)
				{
					*((short *)(&buffer[bufcount])) = (short)(s_new - s_old);
					s_old = s_new;
				} else
				{
					*((short *)(&buffer[bufcount])) = (short)(s_new + s_ofs);
				}
				bufcount += 2;
				if (bufcount >= sizeof(buffer) - 1)
				{
					fwrite(buffer, 1, bufcount, f);
					bufcount = 0;
				}
			}
			if (bufcount) fwrite(buffer, 1, bufcount, f);
		}
		break;


	case RS_STPCM8S:
	case RS_STPCM8U:
	case RS_STPCM8D:
		{
			int s_ofs = (nFlags == RS_STPCM8U) ? 0x80 : 0;
			for (UINT iCh=0; iCh<2; iCh++)
			{
				signed char *p = pSample + iCh;
				int s_old = 0;

				bufcount = 0;
				for (UINT j=0; j<nLen; j++)
				{
					int s_new = *p;
					p += 2;
					if (nFlags == RS_STPCM8D)
					{
						buffer[bufcount++] = (signed char)(s_new - s_old);
						s_old = s_new;
					} else
					{
						buffer[bufcount++] = (signed char)(s_new + s_ofs);
					}
					if (bufcount >= sizeof(buffer))
					{
						fwrite(buffer, 1, bufcount, f);
						bufcount = 0;
					}
				}
				if (bufcount) fwrite(buffer, 1, bufcount, f);
			}
		}
		len = nLen * 2;
		break;

	case RS_STPCM16S:
	case RS_STPCM16U:
	case RS_STPCM16D:
		{
			int s_ofs = (nFlags == RS_STPCM16U) ? 0x8000 : 0;
			for (UINT iCh=0; iCh<2; iCh++)
			{
				signed short *p = ((signed short *)pSample) + iCh;
				int s_old = 0;

				bufcount = 0;
				for (UINT j=0; j<nLen; j++)
				{
					int s_new = *p;
					p += 2;
					if (nFlags == RS_STPCM16D)
					{
						*((short *)(&buffer[bufcount])) = (short)(s_new - s_old);
						s_old = s_new;
					} else
					{
						*((short *)(&buffer[bufcount])) = (short)(s_new + s_ofs);
					}
					bufcount += 2;
					if (bufcount >= sizeof(buffer))
					{
						fwrite(buffer, 1, bufcount, f);
						bufcount = 0;
					}
				}
				if (bufcount) fwrite(buffer, 1, bufcount, f);
			}
		}
		len = nLen*4;
		break;

	case RS_STIPCM8S:
	case RS_STIPCM16S:
		len = nLen * 2;
		if (nFlags == RS_STIPCM16S) len *= 2;
		fwrite(pSample, 1, len, f);
		break;

	default:
		len = nLen;
		bufcount = 0;
		{
			signed char *p = pSample;
			int sinc = (pins->uFlags & CHN_16BIT) ? 2 : 1;
			int s_old = 0, s_ofs = (nFlags == RS_PCM8U) ? 0x80 : 0;
			if (pins->uFlags & CHN_16BIT) p++;
			for (UINT j=0; j<len; j++)
			{
				int s_new = (signed char)(*p);
				p += sinc;
				if (pins->uFlags & CHN_STEREO)
				{
					s_new = (s_new + ((int)*p) + 1) >> 1;
					p += sinc;
				}
				if (nFlags == RS_PCM8D)
				{
					buffer[bufcount++] = (signed char)(s_new - s_old);
					s_old = s_new;
				} else
				{
					buffer[bufcount++] = (signed char)(s_new + s_ofs);
				}
				if (bufcount >= sizeof(buffer))
				{
					fwrite(buffer, 1, bufcount, f);
					bufcount = 0;
				}
			}
			if (bufcount) fwrite(buffer, 1, bufcount, f);
		}
	}
	return len;
}
