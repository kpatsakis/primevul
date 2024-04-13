ex_z(exarg_T *eap)
{
    char_u	*x;
    long	bigness;
    char_u	*kind;
    int		minus = 0;
    linenr_T	start, end, curs, i;
    int		j;
    linenr_T	lnum = eap->line2;

    // Vi compatible: ":z!" uses display height, without a count uses
    // 'scroll'
    if (eap->forceit)
	bigness = Rows - 1;
    else if (!ONE_WINDOW)
	bigness = curwin->w_height - 3;
    else
	bigness = curwin->w_p_scr * 2;
    if (bigness < 1)
	bigness = 1;

    x = eap->arg;
    kind = x;
    if (*kind == '-' || *kind == '+' || *kind == '='
					      || *kind == '^' || *kind == '.')
	++x;
    while (*x == '-' || *x == '+')
	++x;

    if (*x != 0)
    {
	if (!VIM_ISDIGIT(*x))
	{
	    emsg(_(e_non_numeric_argument_to_z));
	    return;
	}
	else
	{
	    bigness = atol((char *)x);

	    // bigness could be < 0 if atol(x) overflows.
	    if (bigness > 2 * curbuf->b_ml.ml_line_count || bigness < 0)
		bigness = 2 * curbuf->b_ml.ml_line_count;

	    p_window = bigness;
	    if (*kind == '=')
		bigness += 2;
	}
    }

    // the number of '-' and '+' multiplies the distance
    if (*kind == '-' || *kind == '+')
	for (x = kind + 1; *x == *kind; ++x)
	    ;

    switch (*kind)
    {
	case '-':
	    start = lnum - bigness * (linenr_T)(x - kind) + 1;
	    end = start + bigness - 1;
	    curs = end;
	    break;

	case '=':
	    start = lnum - (bigness + 1) / 2 + 1;
	    end = lnum + (bigness + 1) / 2 - 1;
	    curs = lnum;
	    minus = 1;
	    break;

	case '^':
	    start = lnum - bigness * 2;
	    end = lnum - bigness;
	    curs = lnum - bigness;
	    break;

	case '.':
	    start = lnum - (bigness + 1) / 2 + 1;
	    end = lnum + (bigness + 1) / 2 - 1;
	    curs = end;
	    break;

	default:  // '+'
	    start = lnum;
	    if (*kind == '+')
		start += bigness * (linenr_T)(x - kind - 1) + 1;
	    else if (eap->addr_count == 0)
		++start;
	    end = start + bigness - 1;
	    curs = end;
	    break;
    }

    if (start < 1)
	start = 1;

    if (end > curbuf->b_ml.ml_line_count)
	end = curbuf->b_ml.ml_line_count;

    if (curs > curbuf->b_ml.ml_line_count)
	curs = curbuf->b_ml.ml_line_count;
    else if (curs < 1)
	curs = 1;

    for (i = start; i <= end; i++)
    {
	if (minus && i == lnum)
	{
	    msg_putchar('\n');

	    for (j = 1; j < Columns; j++)
		msg_putchar('-');
	}

	print_line(i, eap->flags & EXFLAG_NR, eap->flags & EXFLAG_LIST);

	if (minus && i == lnum)
	{
	    msg_putchar('\n');

	    for (j = 1; j < Columns; j++)
		msg_putchar('-');
	}
    }

    if (curwin->w_cursor.lnum != curs)
    {
	curwin->w_cursor.lnum = curs;
	curwin->w_cursor.col = 0;
    }
    ex_no_reprint = TRUE;
}