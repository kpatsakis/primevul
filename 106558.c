boolop(PG_FUNCTION_ARGS)
{
	ArrayType  *val = PG_GETARG_ARRAYTYPE_P_COPY(0);
	QUERYTYPE  *query = PG_GETARG_QUERYTYPE_P(1);
	CHKVAL		chkval;
	bool		result;

	CHECKARRVALID(val);
	PREPAREARR(val);
	chkval.arrb = ARRPTR(val);
	chkval.arre = chkval.arrb + ARRNELEMS(val);
	result = execute(GETQUERY(query) + query->size - 1,
					 &chkval, true,
					 checkcondition_arr);
	pfree(val);

	PG_FREE_IF_COPY(query, 1);
	PG_RETURN_BOOL(result);
}
