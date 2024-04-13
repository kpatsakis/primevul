hstore_in(PG_FUNCTION_ARGS)
{
	HSParser	state;
	int32		buflen;
	HStore	   *out;

	state.begin = PG_GETARG_CSTRING(0);

	parse_hstore(&state);

	state.pcur = hstoreUniquePairs(state.pairs, state.pcur, &buflen);

	out = hstorePairs(state.pairs, state.pcur, buflen);

	PG_RETURN_POINTER(out);
}
