BOOL CSoundFile::CanPackSample(LPSTR pSample, UINT nLen, UINT nPacking, BYTE *result)
{
	int pos, old, oldpos, besttable = 0;
	DWORD dwErr, dwTotal, dwResult;
	int i,j;

	if (result) *result = 0;
	if ((!pSample) || (nLen < 1024)) return FALSE;
	dwResult = 0;
	for (j=1; j<MAX_PACK_TABLES; j++)
	{
		memcpy(CompressionTable, UnpackTable[j], 16);
		dwErr = 0;
		dwTotal = 1;
		old = pos = oldpos = 0;
		for (i=0; i<(int)nLen; i++)
		{
			int s = (int)pSample[i];
			PackSample(pos, s);
			dwErr += abs(pos - oldpos);
			dwTotal += abs(s - old);
			old = s;
			oldpos = pos;
		}
		dwErr = _muldiv(dwErr, 100, dwTotal);
		if (dwErr >= dwResult)
		{
			dwResult = dwErr;
			besttable = j;
		}
	}
	memcpy(CompressionTable, UnpackTable[besttable], 16);
	if (result)
	{
		if (dwResult > 100) *result	= 100; else *result = (BYTE)dwResult;
	}
	return (dwResult >= nPacking) ? TRUE : FALSE;
}
