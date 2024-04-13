linelen(int *has_tab)
{
    char_u  *line;
    char_u  *first;
    char_u  *last;
    int	    save;
    int	    len;

    // Get the line.  If it's empty bail out early (could be the empty string
    // for an unloaded buffer).
    line = ml_get_curline();
    if (*line == NUL)
	return 0;

    // find the first non-blank character
    first = skipwhite(line);

    // find the character after the last non-blank character
    for (last = first + STRLEN(first);
				last > first && VIM_ISWHITE(last[-1]); --last)
	;
    save = *last;
    *last = NUL;
    len = linetabsize(line);		// get line length
    if (has_tab != NULL)		// check for embedded TAB
	*has_tab = (vim_strchr(first, TAB) != NULL);
    *last = save;

    return len;
}