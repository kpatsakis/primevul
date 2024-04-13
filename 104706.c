UINT CSoundFile::GetNumPatterns() const
{
	UINT i = 0;
	while ((i < MAX_ORDERS) && (Order[i] < 0xFF)) i++;
	return i;
}
