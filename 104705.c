UINT CSoundFile::GetNumInstruments() const
{
	UINT n=0;
	for (UINT i=0; i<MAX_INSTRUMENTS; i++) if (Ins[i].pSample) n++;
	return n;
}
