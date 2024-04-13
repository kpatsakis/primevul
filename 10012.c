print_line_no_prefix(
    linenr_T	lnum,
    int		use_number,
    int		list)
{
    char	numbuf[30];

    if (curwin->w_p_nu || use_number)
    {
	vim_snprintf(numbuf, sizeof(numbuf),
				   "%*ld ", number_width(curwin), (long)lnum);
	msg_puts_attr(numbuf, HL_ATTR(HLF_N));	// Highlight line nrs
    }
    msg_prt_line(ml_get(lnum), list);
}