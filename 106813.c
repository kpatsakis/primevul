gettoken_query(TSQueryParserState state,
			   int8 *operator,
			   int *lenval, char **strval, int16 *weight, bool *prefix)
{
	*weight = 0;
	*prefix = false;

	while (1)
	{
		switch (state->state)
		{
			case WAITFIRSTOPERAND:
			case WAITOPERAND:
				if (t_iseq(state->buf, '!'))
				{
					(state->buf)++;		/* can safely ++, t_iseq guarantee
										 * that pg_mblen()==1 */
					*operator = OP_NOT;
					state->state = WAITOPERAND;
					return PT_OPR;
				}
				else if (t_iseq(state->buf, '('))
				{
					state->count++;
					(state->buf)++;
					state->state = WAITOPERAND;
					return PT_OPEN;
				}
				else if (t_iseq(state->buf, ':'))
				{
					ereport(ERROR,
							(errcode(ERRCODE_SYNTAX_ERROR),
							 errmsg("syntax error in tsquery: \"%s\"",
									state->buffer)));
				}
				else if (!t_isspace(state->buf))
				{
					/*
					 * We rely on the tsvector parser to parse the value for
					 * us
					 */
					reset_tsvector_parser(state->valstate, state->buf);
					if (gettoken_tsvector(state->valstate, strval, lenval, NULL, NULL, &state->buf))
					{
						state->buf = get_modifiers(state->buf, weight, prefix);
						state->state = WAITOPERATOR;
						return PT_VAL;
					}
					else if (state->state == WAITFIRSTOPERAND)
						return PT_END;
					else
						ereport(ERROR,
								(errcode(ERRCODE_SYNTAX_ERROR),
								 errmsg("no operand in tsquery: \"%s\"",
										state->buffer)));
				}
				break;
			case WAITOPERATOR:
				if (t_iseq(state->buf, '&'))
				{
					state->state = WAITOPERAND;
					*operator = OP_AND;
					(state->buf)++;
					return PT_OPR;
				}
				if (t_iseq(state->buf, '|'))
				{
					state->state = WAITOPERAND;
					*operator = OP_OR;
					(state->buf)++;
					return PT_OPR;
				}
				else if (t_iseq(state->buf, ')'))
				{
					(state->buf)++;
					state->count--;
					return (state->count < 0) ? PT_ERR : PT_CLOSE;
				}
				else if (*(state->buf) == '\0')
					return (state->count) ? PT_ERR : PT_END;
				else if (!t_isspace(state->buf))
					return PT_ERR;
				break;
			case WAITSINGLEOPERAND:
				if (*(state->buf) == '\0')
					return PT_END;
				*strval = state->buf;
				*lenval = strlen(state->buf);
				state->buf += strlen(state->buf);
				state->count++;
				return PT_VAL;
			default:
				return PT_ERR;
				break;
		}
		state->buf += pg_mblen(state->buf);
	}
}
