parse_hstore(HSParser *state)
{
	int			st = WKEY;
	bool		escaped = false;

	state->plen = 16;
	state->pairs = (Pairs *) palloc(sizeof(Pairs) * state->plen);
	state->pcur = 0;
	state->ptr = state->begin;
	state->word = NULL;

	while (1)
	{
		if (st == WKEY)
		{
			if (!get_val(state, false, &escaped))
				return;
			if (state->pcur >= state->plen)
			{
				state->plen *= 2;
				state->pairs = (Pairs *) repalloc(state->pairs, sizeof(Pairs) * state->plen);
			}
			state->pairs[state->pcur].key = state->word;
			state->pairs[state->pcur].keylen = hstoreCheckKeyLen(state->cur - state->word);
			state->pairs[state->pcur].val = NULL;
			state->word = NULL;
			st = WEQ;
		}
		else if (st == WEQ)
		{
			if (*(state->ptr) == '=')
			{
				st = WGT;
			}
			else if (*(state->ptr) == '\0')
			{
				elog(ERROR, "Unexpected end of string");
			}
			else if (!isspace((unsigned char) *(state->ptr)))
			{
				elog(ERROR, "Syntax error near '%c' at position %d", *(state->ptr), (int32) (state->ptr - state->begin));
			}
		}
		else if (st == WGT)
		{
			if (*(state->ptr) == '>')
			{
				st = WVAL;
			}
			else if (*(state->ptr) == '\0')
			{
				elog(ERROR, "Unexpected end of string");
			}
			else
			{
				elog(ERROR, "Syntax error near '%c' at position %d", *(state->ptr), (int32) (state->ptr - state->begin));
			}
		}
		else if (st == WVAL)
		{
			if (!get_val(state, true, &escaped))
				elog(ERROR, "Unexpected end of string");
			state->pairs[state->pcur].val = state->word;
			state->pairs[state->pcur].vallen = hstoreCheckValLen(state->cur - state->word);
			state->pairs[state->pcur].isnull = false;
			state->pairs[state->pcur].needfree = true;
			if (state->cur - state->word == 4 && !escaped)
			{
				state->word[4] = '\0';
				if (0 == pg_strcasecmp(state->word, "null"))
					state->pairs[state->pcur].isnull = true;
			}
			state->word = NULL;
			state->pcur++;
			st = WDEL;
		}
		else if (st == WDEL)
		{
			if (*(state->ptr) == ',')
			{
				st = WKEY;
			}
			else if (*(state->ptr) == '\0')
			{
				return;
			}
			else if (!isspace((unsigned char) *(state->ptr)))
			{
				elog(ERROR, "Syntax error near '%c' at position %d", *(state->ptr), (int32) (state->ptr - state->begin));
			}
		}
		else
			elog(ERROR, "Unknown state %d at line %d in file '%s'", st, __LINE__, __FILE__);

		state->ptr++;
	}
}
