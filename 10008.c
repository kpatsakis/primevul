sort_compare(const void *s1, const void *s2)
{
    sorti_T	l1 = *(sorti_T *)s1;
    sorti_T	l2 = *(sorti_T *)s2;
    int		result = 0;

    // If the user interrupts, there's no way to stop qsort() immediately, but
    // if we return 0 every time, qsort will assume it's done sorting and
    // exit.
    if (sort_abort)
	return 0;
    fast_breakcheck();
    if (got_int)
	sort_abort = TRUE;

    if (sort_nr)
    {
	if (l1.st_u.num.is_number != l2.st_u.num.is_number)
	    result = l1.st_u.num.is_number - l2.st_u.num.is_number;
	else
	    result = l1.st_u.num.value == l2.st_u.num.value ? 0
			     : l1.st_u.num.value > l2.st_u.num.value ? 1 : -1;
    }
#ifdef FEAT_FLOAT
    else if (sort_flt)
	result = l1.st_u.value_flt == l2.st_u.value_flt ? 0
			     : l1.st_u.value_flt > l2.st_u.value_flt ? 1 : -1;
#endif
    else
    {
	// We need to copy one line into "sortbuf1", because there is no
	// guarantee that the first pointer becomes invalid when obtaining the
	// second one.
	STRNCPY(sortbuf1, ml_get(l1.lnum) + l1.st_u.line.start_col_nr,
		     l1.st_u.line.end_col_nr - l1.st_u.line.start_col_nr + 1);
	sortbuf1[l1.st_u.line.end_col_nr - l1.st_u.line.start_col_nr] = 0;
	STRNCPY(sortbuf2, ml_get(l2.lnum) + l2.st_u.line.start_col_nr,
		     l2.st_u.line.end_col_nr - l2.st_u.line.start_col_nr + 1);
	sortbuf2[l2.st_u.line.end_col_nr - l2.st_u.line.start_col_nr] = 0;

	result = string_compare(sortbuf1, sortbuf2);
    }

    // If two lines have the same value, preserve the original line order.
    if (result == 0)
	return (int)(l1.lnum - l2.lnum);
    return result;
}