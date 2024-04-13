line_send(PG_FUNCTION_ARGS)
{
	LINE	   *line = PG_GETARG_LINE_P(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, line->A);
	pq_sendfloat8(&buf, line->B);
	pq_sendfloat8(&buf, line->C);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}
