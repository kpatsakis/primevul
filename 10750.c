fix_indent(void)
{
    if (p_paste)
	return;
# ifdef FEAT_LISP
    if (curbuf->b_p_lisp && curbuf->b_p_ai)
	fixthisline(get_lisp_indent);
# endif
# if defined(FEAT_LISP) && defined(FEAT_CINDENT)
    else
# endif
# ifdef FEAT_CINDENT
	if (cindent_on())
	    do_c_expr_indent();
# endif
}