SetupWildMatch(FontTablePtr table, FontNamePtr pat,
	       int *leftp, int *rightp, int *privatep)
{
    int         nDashes;
    char        c;
    char       *t;
    char       *firstWild;
    char       *firstDigit;
    int         first;
    int         center,
                left,
                right;
    int         result;
    char	*name;

    name = pat->name;
    nDashes = pat->ndashes;
    firstWild = 0;
    firstDigit = 0;
    t = name;
    while ((c = *t++)) {
	if (isWild(c)) {
	    if (!firstWild)
		firstWild = t - 1;
	}
	if (isDigit(c)) {
	    if (!firstDigit)
		firstDigit = t - 1;
	}
    }
    left = 0;
    right = table->used;
    if (firstWild)
	*privatep = nDashes;
    else
	*privatep = -1;
    if (!table->sorted) {
	*leftp = left;
	*rightp = right;
	return -1;
    } else if (firstWild) {
	if (firstDigit && firstDigit < firstWild)
	    first = firstDigit - name;
	else
	    first = firstWild - name;
	while (left < right) {
	    center = (left + right) / 2;
	    result = strncmp(name, table->entries[center].name.name, first);
	    if (result == 0)
		break;
	    if (result < 0)
		right = center;
	    else
		left = center + 1;
	}
	*leftp = left;
	*rightp = right;
	return -1;
    } else {
	while (left < right) {
	    center = (left + right) / 2;
	    result = strcmpn(name, table->entries[center].name.name);
	    if (result == 0)
		return center;
	    if (result < 0)
		right = center;
	    else
		left = center + 1;
	}
	*leftp = 1;
	*rightp = 0;
	return -1;
    }
}
