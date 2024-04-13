setup_firstcall(FuncCallContext *funcctx, HStore *hs,
				FunctionCallInfoData *fcinfo)
{
	MemoryContext oldcontext;
	HStore	   *st;

	oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

	st = (HStore *) palloc(VARSIZE(hs));
	memcpy(st, hs, VARSIZE(hs));

	funcctx->user_fctx = (void *) st;

	if (fcinfo)
	{
		TupleDesc	tupdesc;

		/* Build a tuple descriptor for our result type */
		if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
			elog(ERROR, "return type must be a row type");

		funcctx->tuple_desc = BlessTupleDesc(tupdesc);
	}

	MemoryContextSwitchTo(oldcontext);
}
