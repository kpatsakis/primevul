get_val(HSParser *state, bool ignoreeq, bool *escaped)
{
	int			st = GV_WAITVAL;

	state->wordlen = 32;
	state->cur = state->word = palloc(state->wordlen);
	*escaped = false;

	while (1)
	{
		if (st == GV_WAITVAL)
		{
			if (*(state->ptr) == '"')
			{
				*escaped = true;
				st = GV_INESCVAL;
			}
			else if (*(state->ptr) == '\0')
			{
				return false;
			}
			else if (*(state->ptr) == '=' && !ignoreeq)
			{
				elog(ERROR, "Syntax error near '%c' at position %d", *(state->ptr), (int32) (state->ptr - state->begin));
			}
			else if (*(state->ptr) == '\\')
			{
				st = GV_WAITESCIN;
			}
			else if (!isspace((unsigned char) *(state->ptr)))
			{
				*(state->cur) = *(state->ptr);
				state->cur++;
				st = GV_INVAL;
			}
		}
		else if (st == GV_INVAL)
		{
			if (*(state->ptr) == '\\')
			{
				st = GV_WAITESCIN;
			}
			else if (*(state->ptr) == '=' && !ignoreeq)
			{
				state->ptr--;
				return true;
			}
			else if (*(state->ptr) == ',' && ignoreeq)
			{
				state->ptr--;
				return true;
			}
			else if (isspace((unsigned char) *(state->ptr)))
			{
				return true;
			}
			else if (*(state->ptr) == '\0')
			{
				state->ptr--;
				return true;
			}
			else
			{
				RESIZEPRSBUF;
				*(state->cur) = *(state->ptr);
				state->cur++;
			}
		}
		else if (st == GV_INESCVAL)
		{
			if (*(state->ptr) == '\\')
			{
				st = GV_WAITESCESCIN;
			}
			else if (*(state->ptr) == '"')
			{
				return true;
			}
			else if (*(state->ptr) == '\0')
			{
				elog(ERROR, "Unexpected end of string");
			}
			else
			{
				RESIZEPRSBUF;
				*(state->cur) = *(state->ptr);
				state->cur++;
			}
		}
		else if (st == GV_WAITESCIN)
		{
			if (*(state->ptr) == '\0')
				elog(ERROR, "Unexpected end of string");
			RESIZEPRSBUF;
			*(state->cur) = *(state->ptr);
			state->cur++;
			st = GV_INVAL;
		}
		else if (st == GV_WAITESCESCIN)
		{
			if (*(state->ptr) == '\0')
				elog(ERROR, "Unexpected end of string");
			RESIZEPRSBUF;
			*(state->cur) = *(state->ptr);
			state->cur++;
			st = GV_INESCVAL;
		}
		else
			elog(ERROR, "Unknown state %d at position line %d in file '%s'", st, __LINE__, __FILE__);

		state->ptr++;
	}
}
