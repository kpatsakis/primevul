find_pipe(char_u *cmd)
{
    char_u  *p;
    int	    inquote = FALSE;

    for (p = cmd; *p != NUL; ++p)
    {
	if (!inquote && *p == '|')
	    return p;
	if (*p == '"')
	    inquote = !inquote;
	else if (rem_backslash(p))
	    ++p;
    }
    return NULL;
}