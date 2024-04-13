pushquery(WORKSTATE *state, int32 type, int32 val)
{
	NODE	   *tmp = (NODE *) palloc(sizeof(NODE));

	tmp->type = type;
	tmp->val = val;
	tmp->next = state->str;
	state->str = tmp;
	state->num++;
}
