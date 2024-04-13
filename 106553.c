hstore_svals(PG_FUNCTION_ARGS)
{
	FuncCallContext *funcctx;
	HStore	   *hs;
	int			i;

	if (SRF_IS_FIRSTCALL())
	{
		hs = PG_GETARG_HS(0);
		funcctx = SRF_FIRSTCALL_INIT();
		setup_firstcall(funcctx, hs, NULL);
	}

	funcctx = SRF_PERCALL_SETUP();
	hs = (HStore *) funcctx->user_fctx;
	i = funcctx->call_cntr;

	if (i < HS_COUNT(hs))
	{
		HEntry	   *entries = ARRPTR(hs);

		if (HS_VALISNULL(entries, i))
		{
			ReturnSetInfo *rsi;

			/* ugly ugly ugly. why no macro for this? */
			(funcctx)->call_cntr++;
			rsi = (ReturnSetInfo *) fcinfo->resultinfo;
			rsi->isDone = ExprMultipleResult;
			PG_RETURN_NULL();
		}
		else
		{
			text	   *item;

			item = cstring_to_text_with_len(HS_VAL(entries, STRPTR(hs), i),
											HS_VALLEN(entries, i));

			SRF_RETURN_NEXT(funcctx, PointerGetDatum(item));
		}
	}

	SRF_RETURN_DONE(funcctx);
}
