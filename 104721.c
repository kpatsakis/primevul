BOOL CSoundFile::SetPatternName(UINT nPat, LPCSTR lpszName)
{
        char szName[MAX_PATTERNNAME] = "";   // changed from CHAR
	if (nPat >= MAX_PATTERNS) return FALSE;
	if (lpszName) lstrcpyn(szName, lpszName, MAX_PATTERNNAME);
	szName[MAX_PATTERNNAME-1] = 0;
	if (!m_lpszPatternNames) m_nPatternNames = 0;
	if (nPat >= m_nPatternNames)
	{
		if (!lpszName[0]) return TRUE;
		UINT len = (nPat+1)*MAX_PATTERNNAME;
		char *p = new char[len];   // changed from CHAR
		if (!p) return FALSE;
		memset(p, 0, len);
		if (m_lpszPatternNames)
		{
			memcpy(p, m_lpszPatternNames, m_nPatternNames * MAX_PATTERNNAME);
			delete m_lpszPatternNames;
			m_lpszPatternNames = NULL;
		}
		m_lpszPatternNames = p;
		m_nPatternNames = nPat + 1;
	}
	memcpy(m_lpszPatternNames + nPat * MAX_PATTERNNAME, szName, MAX_PATTERNNAME);
	return TRUE;
}
