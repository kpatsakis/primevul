op_reindent(oparg_T *oap, int (*how)(void))
{
    long	i;
    char_u	*l;
    int		amount;
    linenr_T	first_changed = 0;
    linenr_T	last_changed = 0;
    linenr_T	start_lnum = curwin->w_cursor.lnum;

    // Don't even try when 'modifiable' is off.
    if (!curbuf->b_p_ma)
    {
	emsg(_(e_cannot_make_changes_modifiable_is_off));
	return;
    }

    for (i = oap->line_count; --i >= 0 && !got_int; )
    {
	// it's a slow thing to do, so give feedback so there's no worry that
	// the computer's just hung.

	if (i > 1
		&& (i % 50 == 0 || i == oap->line_count - 1)
		&& oap->line_count > p_report)
	    smsg(_("%ld lines to indent... "), i);

	// Be vi-compatible: For lisp indenting the first line is not
	// indented, unless there is only one line.
# ifdef FEAT_LISP
	if (i != oap->line_count - 1 || oap->line_count == 1
						    || how != get_lisp_indent)
# endif
	{
	    l = skipwhite(ml_get_curline());
	    if (*l == NUL)		    // empty or blank line
		amount = 0;
	    else
		amount = how();		    // get the indent for this line

	    if (amount >= 0 && set_indent(amount, SIN_UNDO))
	    {
		// did change the indent, call changed_lines() later
		if (first_changed == 0)
		    first_changed = curwin->w_cursor.lnum;
		last_changed = curwin->w_cursor.lnum;
	    }
	}
	++curwin->w_cursor.lnum;
	curwin->w_cursor.col = 0;  // make sure it's valid
    }

    // put cursor on first non-blank of indented line
    curwin->w_cursor.lnum = start_lnum;
    beginline(BL_SOL | BL_FIX);

    // Mark changed lines so that they will be redrawn.  When Visual
    // highlighting was present, need to continue until the last line.  When
    // there is no change still need to remove the Visual highlighting.
    if (last_changed != 0)
	changed_lines(first_changed, 0,
		oap->is_VIsual ? start_lnum + oap->line_count :
		last_changed + 1, 0L);
    else if (oap->is_VIsual)
	redraw_curbuf_later(INVERTED);

    if (oap->line_count > p_report)
    {
	i = oap->line_count - (i + 1);
	smsg(NGETTEXT("%ld line indented ",
						 "%ld lines indented ", i), i);
    }
    if ((cmdmod.cmod_flags & CMOD_LOCKMARKS) == 0)
    {
	// set '[ and '] marks
	curbuf->b_op_start = oap->start;
	curbuf->b_op_end = oap->end;
    }
}