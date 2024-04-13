gettoken_query(QPRS_STATE *state, int32 *val, int32 *lenval, char **strval, uint16 *flag)
{
	int			charlen;

	for (;;)
	{
		charlen = pg_mblen(state->buf);

		switch (state->state)
		{
			case WAITOPERAND:
				if (charlen == 1 && t_iseq(state->buf, '!'))
				{
					(state->buf)++;
					*val = (int32) '!';
					return OPR;
				}
				else if (charlen == 1 && t_iseq(state->buf, '('))
				{
					state->count++;
					(state->buf)++;
					return OPEN;
				}
				else if (ISALNUM(state->buf))
				{
					state->state = INOPERAND;
					*strval = state->buf;
					*lenval = charlen;
					*flag = 0;
				}
				else if (!t_isspace(state->buf))
					ereport(ERROR,
							(errcode(ERRCODE_SYNTAX_ERROR),
							 errmsg("operand syntax error")));
				break;
			case INOPERAND:
				if (ISALNUM(state->buf))
				{
					if (*flag)
						ereport(ERROR,
								(errcode(ERRCODE_SYNTAX_ERROR),
								 errmsg("modificators syntax error")));
					*lenval += charlen;
				}
				else if (charlen == 1 && t_iseq(state->buf, '%'))
					*flag |= LVAR_SUBLEXEME;
				else if (charlen == 1 && t_iseq(state->buf, '@'))
					*flag |= LVAR_INCASE;
				else if (charlen == 1 && t_iseq(state->buf, '*'))
					*flag |= LVAR_ANYEND;
				else
				{
					state->state = WAITOPERATOR;
					return VAL;
				}
				break;
			case WAITOPERATOR:
				if (charlen == 1 && (t_iseq(state->buf, '&') || t_iseq(state->buf, '|')))
				{
					state->state = WAITOPERAND;
					*val = (int32) *(state->buf);
					(state->buf)++;
					return OPR;
				}
				else if (charlen == 1 && t_iseq(state->buf, ')'))
				{
					(state->buf)++;
					state->count--;
					return (state->count < 0) ? ERR : CLOSE;
				}
				else if (*(state->buf) == '\0')
					return (state->count) ? ERR : END;
				else if (charlen == 1 && !t_iseq(state->buf, ' '))
					return ERR;
				break;
			default:
				return ERR;
				break;
		}

		state->buf += charlen;
	}
}
