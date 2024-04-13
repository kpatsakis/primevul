UINT CSoundFile::GetSampleName(UINT nSample,LPSTR s) const
{
        char sztmp[40] = "";      // changed from CHAR
	memcpy(sztmp, m_szNames[nSample],32);
	sztmp[31] = 0;
	if (s) strcpy(s, sztmp);
	return strlen(sztmp);
}
