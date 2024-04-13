line_recv(PG_FUNCTION_ARGS)
{
	StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
	LINE	   *line;

	line = (LINE *) palloc(sizeof(LINE));

	line->A = pq_getmsgfloat8(buf);
	line->B = pq_getmsgfloat8(buf);
	line->C = pq_getmsgfloat8(buf);

	PG_RETURN_LINE_P(line);
}
