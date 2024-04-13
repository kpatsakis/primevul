execconsistent(QUERYTYPE *query, ArrayType *array, bool calcnot)
{
	CHKVAL		chkval;

	CHECKARRVALID(array);
	chkval.arrb = ARRPTR(array);
	chkval.arre = chkval.arrb + ARRNELEMS(array);
	return execute(GETQUERY(query) + query->size - 1,
				   (void *) &chkval, calcnot,
				   checkcondition_arr);
}
