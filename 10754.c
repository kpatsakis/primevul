tabstop_start(colnr_T col, int ts, int *vts)
{
    int		tabcount;
    colnr_T	tabcol = 0;
    int		t;
    int         excess;

    if (vts == NULL || vts[0] == 0)
	return (col / ts) * ts;

    tabcount = vts[0];
    for (t = 1; t <= tabcount; ++t)
    {
	tabcol += vts[t];
	if (tabcol > col)
	    return tabcol - vts[t];
    }

    excess = tabcol % vts[tabcount];
    return excess + ((col - excess) / vts[tabcount]) * vts[tabcount];
}