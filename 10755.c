get_breakindent_win(
    win_T	*wp,
    char_u	*line) // start of the line
{
    static int	    prev_indent = 0;	// cached indent value
    static long	    prev_ts     = 0L;	// cached tabstop value
    static char_u   *prev_line = NULL;	// cached pointer to line
    static varnumber_T prev_tick = 0;   // changedtick of cached value
# ifdef FEAT_VARTABS
    static int      *prev_vts = NULL;   // cached vartabs values
# endif
    static int      prev_list = 0;	// cached list value
    static int      prev_listopt = 0;	// cached w_p_briopt_list value
    // cached formatlistpat value
    static char_u   *prev_flp = NULL;
    int		    bri = 0;
    // window width minus window margin space, i.e. what rests for text
    const int	    eff_wwidth = wp->w_width
			    - ((wp->w_p_nu || wp->w_p_rnu)
				&& (vim_strchr(p_cpo, CPO_NUMCOL) == NULL)
						? number_width(wp) + 1 : 0);

    // used cached indent, unless
    // - line pointer changed
    // - 'tabstop' changed
    // - 'briopt_list changed' changed or
    // - 'formatlistpattern' changed
    if (prev_line != line || prev_ts != wp->w_buffer->b_p_ts
	    || prev_tick != CHANGEDTICK(wp->w_buffer)
	    || prev_listopt != wp->w_briopt_list
	    || (prev_flp == NULL
		|| (STRCMP(prev_flp, get_flp_value(wp->w_buffer)) != 0))
# ifdef FEAT_VARTABS
	    || prev_vts != wp->w_buffer->b_p_vts_array
# endif
	)
    {
	prev_line = line;
	prev_ts = wp->w_buffer->b_p_ts;
	prev_tick = CHANGEDTICK(wp->w_buffer);
# ifdef FEAT_VARTABS
	prev_vts = wp->w_buffer->b_p_vts_array;
	prev_indent = get_indent_str_vtab(line,
				     (int)wp->w_buffer->b_p_ts,
				    wp->w_buffer->b_p_vts_array, wp->w_p_list);
# else
	prev_indent = get_indent_str(line,
				     (int)wp->w_buffer->b_p_ts, wp->w_p_list);
# endif
	prev_listopt = wp->w_briopt_list;
	prev_list = 0;
	vim_free(prev_flp);
	prev_flp = vim_strsave(get_flp_value(wp->w_buffer));
	// add additional indent for numbered lists
	if (wp->w_briopt_list != 0)
	{
	    regmatch_T	    regmatch;

	    regmatch.regprog = vim_regcomp(prev_flp,
				   RE_MAGIC + RE_STRING + RE_AUTO + RE_STRICT);

	    if (regmatch.regprog != NULL)
	    {
		regmatch.rm_ic = FALSE;
		if (vim_regexec(&regmatch, line, 0))
		{
		    if (wp->w_briopt_list > 0)
			prev_list = wp->w_briopt_list;
		    else
			prev_list = (*regmatch.endp - *regmatch.startp);
		}
		vim_regfree(regmatch.regprog);
	    }
	}
    }
    bri = prev_indent + wp->w_briopt_shift;

    // Add offset for number column, if 'n' is in 'cpoptions'
    bri += win_col_off2(wp);

    // add additional indent for numbered lists
    if (wp->w_briopt_list != 0)
    {
	if (wp->w_briopt_list > 0)
	    bri += prev_list;
	else
	    bri = prev_list;
    }

    // indent minus the length of the showbreak string
    if (wp->w_briopt_sbr)
	bri -= vim_strsize(get_showbreak_value(wp));


    // never indent past left window margin
    if (bri < 0)
	bri = 0;

    // always leave at least bri_min characters on the left,
    // if text width is sufficient
    else if (bri > eff_wwidth - wp->w_briopt_min)
	bri = (eff_wwidth - wp->w_briopt_min < 0)
					   ? 0 : eff_wwidth - wp->w_briopt_min;

    return bri;
}