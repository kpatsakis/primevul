UINT CSoundFile::GetNumChannels() const
{
	UINT n = 0;
	for (UINT i=0; i<m_nChannels; i++) if (ChnSettings[i].nVolume) n++;
	return n;
}
