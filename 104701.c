UINT CSoundFile::GetCurrentPos() const
{
	UINT pos = 0;

	for (UINT i=0; i<m_nCurrentPattern; i++) if (Order[i] < MAX_PATTERNS)
		pos += PatternSize[Order[i]];
	return pos + m_nRow;
}
