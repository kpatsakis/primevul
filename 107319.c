int is_dup(unsigned short id)
{
	int idx = id/8;
	int bit = id % 8;
	unsigned char mask = (1 << bit);

	if (ids[idx] & mask)
		return 1;

	ids[idx] |= mask;
	return 0;
}
