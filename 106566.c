gettoken(WORKSTATE *state, int32 *val)
{
	char		nnn[16];
	int			innn;

	*val = 0;					/* default result */

	innn = 0;
	while (1)
	{
		if (innn >= sizeof(nnn))
			return ERR;			/* buffer overrun => syntax error */
		switch (state->state)
		{
			case WAITOPERAND:
				innn = 0;
				if ((*(state->buf) >= '0' && *(state->buf) <= '9') ||
					*(state->buf) == '-')
				{
					state->state = WAITENDOPERAND;
					nnn[innn++] = *(state->buf);
				}
				else if (*(state->buf) == '!')
				{
					(state->buf)++;
					*val = (int32) '!';
					return OPR;
				}
				else if (*(state->buf) == '(')
				{
					state->count++;
					(state->buf)++;
					return OPEN;
				}
				else if (*(state->buf) != ' ')
					return ERR;
				break;
			case WAITENDOPERAND:
				if (*(state->buf) >= '0' && *(state->buf) <= '9')
				{
					nnn[innn++] = *(state->buf);
				}
				else
				{
					long		lval;

					nnn[innn] = '\0';
					errno = 0;
					lval = strtol(nnn, NULL, 0);
					*val = (int32) lval;
					if (errno != 0 || (long) *val != lval)
						return ERR;
					state->state = WAITOPERATOR;
					return (state->count && *(state->buf) == '\0')
						? ERR : VAL;
				}
				break;
			case WAITOPERATOR:
				if (*(state->buf) == '&' || *(state->buf) == '|')
				{
					state->state = WAITOPERAND;
					*val = (int32) *(state->buf);
					(state->buf)++;
					return OPR;
				}
				else if (*(state->buf) == ')')
				{
					(state->buf)++;
					state->count--;
					return (state->count < 0) ? ERR : CLOSE;
				}
				else if (*(state->buf) == '\0')
					return (state->count) ? ERR : END;
				else if (*(state->buf) != ' ')
					return ERR;
				break;
			default:
				return ERR;
				break;
		}
		(state->buf)++;
	}
}
