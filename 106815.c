makepol(TSQueryParserState state,
		PushFunction pushval,
		Datum opaque)
{
	int8		operator = 0;
	ts_tokentype type;
	int			lenval = 0;
	char	   *strval = NULL;
	int8		opstack[STACKDEPTH];
	int			lenstack = 0;
	int16		weight = 0;
	bool		prefix;

	/* since this function recurses, it could be driven to stack overflow */
	check_stack_depth();

	while ((type = gettoken_query(state, &operator, &lenval, &strval, &weight, &prefix)) != PT_END)
	{
		switch (type)
		{
			case PT_VAL:
				pushval(opaque, state, strval, lenval, weight, prefix);
				while (lenstack && (opstack[lenstack - 1] == OP_AND ||
									opstack[lenstack - 1] == OP_NOT))
				{
					lenstack--;
					pushOperator(state, opstack[lenstack]);
				}
				break;
			case PT_OPR:
				if (lenstack && operator == OP_OR)
					pushOperator(state, OP_OR);
				else
				{
					if (lenstack == STACKDEPTH) /* internal error */
						elog(ERROR, "tsquery stack too small");
					opstack[lenstack] = operator;
					lenstack++;
				}
				break;
			case PT_OPEN:
				makepol(state, pushval, opaque);

				while (lenstack && (opstack[lenstack - 1] == OP_AND ||
									opstack[lenstack - 1] == OP_NOT))
				{
					lenstack--;
					pushOperator(state, opstack[lenstack]);
				}
				break;
			case PT_CLOSE:
				while (lenstack)
				{
					lenstack--;
					pushOperator(state, opstack[lenstack]);
				};
				return;
			case PT_ERR:
			default:
				ereport(ERROR,
						(errcode(ERRCODE_SYNTAX_ERROR),
						 errmsg("syntax error in tsquery: \"%s\"",
								state->buffer)));
		}
	}
	while (lenstack)
	{
		lenstack--;
		pushOperator(state, opstack[lenstack]);
	}
}
