UINT CSoundFile::GetMaxPosition() const
{
	UINT max = 0;
	UINT i = 0;

	while ((i < MAX_ORDERS) && (Order[i] != 0xFF))
	{
		if (Order[i] < MAX_PATTERNS) max += PatternSize[Order[i]];
		i++;
	}
	return max;
}
