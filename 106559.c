bqarr_out(PG_FUNCTION_ARGS)
{
	QUERYTYPE  *query = PG_GETARG_QUERYTYPE_P(0);
	INFIX		nrm;

	if (query->size == 0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("empty query")));

	nrm.curpol = GETQUERY(query) + query->size - 1;
	nrm.buflen = 32;
	nrm.cur = nrm.buf = (char *) palloc(sizeof(char) * nrm.buflen);
	*(nrm.cur) = '\0';
	infix(&nrm, true);

	PG_FREE_IF_COPY(query, 0);
	PG_RETURN_POINTER(nrm.buf);
}
