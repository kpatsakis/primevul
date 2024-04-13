static int strcmpn(const char *s1, const char *s2)
{
    int digits, predigits = 0;
    const char *ss1, *ss2;

    while (1) {
	if (*s1 == 0 && *s2 == 0)
	    return 0;
	digits = Xisdigit(*s1) && Xisdigit(*s2);
	if (digits && !predigits) {
	    ss1 = s1;
	    ss2 = s2;
	    while (Xisdigit(*ss1) && Xisdigit(*ss2))
		ss1++, ss2++;
	    if (!Xisdigit(*ss1) && Xisdigit(*ss2))
		return -1;
	    if (Xisdigit(*ss1) && !Xisdigit(*ss2))
		return 1;
	}
	if ((unsigned char)*s1 < (unsigned char)*s2)
	    return -1;
	if ((unsigned char)*s1 > (unsigned char)*s2)
	    return 1;
	predigits = digits;
	s1++, s2++;
    }
}
