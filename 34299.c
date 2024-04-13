PictOpValid (CARD8 op)
{
    if (/*PictOpMinimum <= op && */ op <= PictOpMaximum)
	return TRUE;
    if (PictOpDisjointMinimum <= op && op <= PictOpDisjointMaximum)
	return TRUE;
    if (PictOpConjointMinimum <= op && op <= PictOpConjointMaximum)
	return TRUE;
    if (PictOpBlendMinimum <= op && op <= PictOpBlendMaximum)
	return TRUE;
    return FALSE;
}
