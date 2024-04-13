skip_vimgrep_pat_ext(char_u *p, char_u **s, int *flags, char_u **nulp, int *cp)
{
    int		c;

    if (vim_isIDc(*p))
    {
	// ":vimgrep pattern fname"
	if (s != NULL)
	    *s = p;
	p = skiptowhite(p);
	if (s != NULL && *p != NUL)
	{
	    if (nulp != NULL)
	    {
		*nulp = p;
		*cp = *p;
	    }
	    *p++ = NUL;
	}
    }
    else
    {
	// ":vimgrep /pattern/[g][j] fname"
	if (s != NULL)
	    *s = p + 1;
	c = *p;
	p = skip_regexp(p + 1, c, TRUE);
	if (*p != c)
	    return NULL;

	// Truncate the pattern.
	if (s != NULL)
	{
	    if (nulp != NULL)
	    {
		*nulp = p;
		*cp = *p;
	    }
	    *p = NUL;
	}
	++p;

	// Find the flags
	while (*p == 'g' || *p == 'j' || *p == 'f')
	{
	    if (flags != NULL)
	    {
		if (*p == 'g')
		    *flags |= VGR_GLOBAL;
		else if (*p == 'j')
		    *flags |= VGR_NOJUMP;
		else
		    *flags |= VGR_FUZZY;
	    }
	    ++p;
	}
    }
    return p;
}