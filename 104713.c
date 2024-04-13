UINT CSoundFile::PackSample(int &sample, int next)
{
	UINT i = 0;
	int delta = next - sample;
	if (delta >= 0)
	{
		for (i=0; i<7; i++) if (delta <= (int)CompressionTable[i+1]) break;
	} else
	{
		for (i=8; i<15; i++) if (delta >= (int)CompressionTable[i+1]) break;
	}
	sample += (int)CompressionTable[i];
	return i;
}
