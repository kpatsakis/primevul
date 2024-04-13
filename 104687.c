MODCOMMAND *CSoundFile::AllocatePattern(UINT rows, UINT nchns)
{
	MODCOMMAND *p = new MODCOMMAND[rows*nchns];
	if (p) memset(p, 0, rows*nchns*sizeof(MODCOMMAND));
	return p;
}
