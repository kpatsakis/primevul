gin_bool_consistent(QUERYTYPE *query, bool *check)
{
	GinChkVal	gcv;
	ITEM	   *items = GETQUERY(query);
	int			i,
				j = 0;

	if (query->size <= 0)
		return FALSE;

	/*
	 * Set up data for checkcondition_gin.	This must agree with the query
	 * extraction code in ginint4_queryextract.
	 */
	gcv.first = items;
	gcv.mapped_check = (bool *) palloc(sizeof(bool) * query->size);
	for (i = 0; i < query->size; i++)
	{
		if (items[i].type == VAL)
			gcv.mapped_check[i] = check[j++];
	}

	return execute(GETQUERY(query) + query->size - 1,
				   (void *) &gcv, true,
				   checkcondition_gin);
}
