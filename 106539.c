hstore_each(PG_FUNCTION_ARGS)
{
	FuncCallContext *funcctx;
	HStore	   *hs;
	int			i;

	if (SRF_IS_FIRSTCALL())
	{
		hs = PG_GETARG_HS(0);
		funcctx = SRF_FIRSTCALL_INIT();
		setup_firstcall(funcctx, hs, fcinfo);
	}

	funcctx = SRF_PERCALL_SETUP();
	hs = (HStore *) funcctx->user_fctx;
	i = funcctx->call_cntr;

	if (i < HS_COUNT(hs))
	{
		HEntry	   *entries = ARRPTR(hs);
		char	   *ptr = STRPTR(hs);
		Datum		res,
					dvalues[2];
		bool		nulls[2] = {false, false};
		text	   *item;
		HeapTuple	tuple;

		item = cstring_to_text_with_len(HS_KEY(entries, ptr, i),
										HS_KEYLEN(entries, i));
		dvalues[0] = PointerGetDatum(item);

		if (HS_VALISNULL(entries, i))
		{
			dvalues[1] = (Datum) 0;
			nulls[1] = true;
		}
		else
		{
			item = cstring_to_text_with_len(HS_VAL(entries, ptr, i),
											HS_VALLEN(entries, i));
			dvalues[1] = PointerGetDatum(item);
		}

		tuple = heap_form_tuple(funcctx->tuple_desc, dvalues, nulls);
		res = HeapTupleGetDatum(tuple);

		SRF_RETURN_NEXT(funcctx, PointerGetDatum(res));
	}

	SRF_RETURN_DONE(funcctx);
}
