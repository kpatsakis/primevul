XcursorLibraryShape (const char *library)
{
    int	low, high;
    int	mid;
    int	c;

    low = 0;
    high = NUM_STANDARD_NAMES - 1;
    while (low < high - 1)
    {
	mid = (low + high) >> 1;
	c = strcmp (library, STANDARD_NAME (mid));
	if (c == 0)
	    return (mid << 1);
	if (c > 0)
	    low = mid;
	else
	    high = mid;
    }
    while (low <= high)
    {
	if (!strcmp (library, STANDARD_NAME (low)))
	    return (low << 1);
	low++;
    }
    return -1;
}
