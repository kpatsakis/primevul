hstore_hash(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	Datum		hval = hash_any((unsigned char *) VARDATA(hs),
								VARSIZE(hs) - VARHDRSZ);

	/*
	 * this is the only place in the code that cares whether the overall
	 * varlena size exactly matches the true data size; this assertion should
	 * be maintained by all the other code, but we make it explicit here.
	 */
	Assert(VARSIZE(hs) ==
		   (HS_COUNT(hs) != 0 ?
			CALCDATASIZE(HS_COUNT(hs),
						 HSE_ENDPOS(ARRPTR(hs)[2 * HS_COUNT(hs) - 1])) :
			HSHRDSIZE));

	PG_FREE_IF_COPY(hs, 0);
	PG_RETURN_DATUM(hval);
}
