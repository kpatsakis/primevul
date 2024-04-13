ex_change(exarg_T *eap)
{
    linenr_T	lnum;

#ifdef FEAT_EVAL
    if (not_in_vim9(eap) == FAIL)
	return;
#endif
    if (eap->line2 >= eap->line1
	    && u_save(eap->line1 - 1, eap->line2 + 1) == FAIL)
	return;

    // the ! flag toggles autoindent
    if (eap->forceit ? !curbuf->b_p_ai : curbuf->b_p_ai)
	append_indent = get_indent_lnum(eap->line1);

    for (lnum = eap->line2; lnum >= eap->line1; --lnum)
    {
	if (curbuf->b_ml.ml_flags & ML_EMPTY)	    // nothing to delete
	    break;
	ml_delete(eap->line1);
    }

    // make sure the cursor is not beyond the end of the file now
    check_cursor_lnum();
    deleted_lines_mark(eap->line1, (long)(eap->line2 - lnum));

    // ":append" on the line above the deleted lines.
    eap->line2 = eap->line1;
    ex_append(eap);
}