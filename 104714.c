BOOL CSoundFile::RemoveSelectedSamples(BOOL *pbIns)
{
	if (!pbIns) return FALSE;
	for (UINT j=1; j<MAX_SAMPLES; j++)
	{
		if ((!pbIns[j]) && (Ins[j].pSample))
		{
			DestroySample(j);
			if ((j == m_nSamples) && (j > 1)) m_nSamples--;
		}
	}
	return TRUE;
}
