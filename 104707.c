BOOL CSoundFile::GetPatternName(UINT nPat, LPSTR lpszName, UINT cbSize) const
{
	if ((!lpszName) || (!cbSize)) return FALSE;
	lpszName[0] = 0;
	if (cbSize > MAX_PATTERNNAME) cbSize = MAX_PATTERNNAME;
	if ((m_lpszPatternNames) && (nPat < m_nPatternNames))
	{
		memcpy(lpszName, m_lpszPatternNames + nPat * MAX_PATTERNNAME, cbSize);
		lpszName[cbSize-1] = 0;
		return TRUE;
	}
	return FALSE;
}
