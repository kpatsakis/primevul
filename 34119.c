CreateColors(
    char		**dataptr,
    unsigned int	 *data_size,
    XpmColor		 *colors,
    unsigned int	  ncolors,
    unsigned int	  cpp)
{
    char buf[BUFSIZ];
    unsigned int a, key, l;
    char *s, *s2;
    char **defaults;

    /* can ncolors be trusted here? */
    for (a = 0; a < ncolors; a++, colors++, dataptr++) {

	defaults = (char **) colors;
	if(sizeof(buf) <= cpp)
	    return(XpmNoMemory);
	strncpy(buf, *defaults++, cpp);
	s = buf + cpp;

	if(sizeof(buf) <= (s-buf))
		return XpmNoMemory;

	for (key = 1; key <= NKEYS; key++, defaults++) {
	    if ((s2 = *defaults)) {
#ifndef VOID_SPRINTF
		s +=
#endif
		/* assume C99 compliance */
			snprintf(s, sizeof(buf)-(s-buf), "\t%s %s", xpmColorKeys[key - 1], s2);
#ifdef VOID_SPRINTF
		s += strlen(s);
#endif
		/* does s point out-of-bounds? */
		if(sizeof(buf) < (s-buf))
			return XpmNoMemory;
	    }
	}
	/* what about using strdup()? */
	l = s - buf + 1;
	s = (char *) XpmMalloc(l);
	if (!s)
	    return (XpmNoMemory);
	*data_size += l;
	*dataptr = strcpy(s, buf);
    }
    return (XpmSuccess);
}
