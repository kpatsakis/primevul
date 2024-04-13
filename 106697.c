line_out(PG_FUNCTION_ARGS)
{
	LINE	   *line = PG_GETARG_LINE_P(0);
	int			ndig = DBL_DIG + extra_float_digits;

	if (ndig < 1)
		ndig = 1;

	PG_RETURN_CSTRING(psprintf("{%.*g,%.*g,%.*g}", ndig, line->A, ndig, line->B, ndig, line->C));
}
