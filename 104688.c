signed char* CSoundFile::AllocateSample(UINT nbytes)
{
	signed char * p = (signed char *)GlobalAllocPtr(GHND, (nbytes+39) & ~7);
	if (p) p += 16;
	return p;
}
