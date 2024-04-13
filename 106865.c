bit_send(PG_FUNCTION_ARGS)
{
	/* Exactly the same as varbit_send, so share code */
	return varbit_send(fcinfo);
}
