fillQT(QTN2QTState *state, QTNode *in)
{
	/* since this function recurses, it could be driven to stack overflow. */
	check_stack_depth();

	if (in->valnode->type == QI_VAL)
	{
		memcpy(state->curitem, in->valnode, sizeof(QueryOperand));

		memcpy(state->curoperand, in->word, in->valnode->qoperand.length);
		state->curitem->qoperand.distance = state->curoperand - state->operand;
		state->curoperand[in->valnode->qoperand.length] = '\0';
		state->curoperand += in->valnode->qoperand.length + 1;
		state->curitem++;
	}
	else
	{
		QueryItem  *curitem = state->curitem;

		Assert(in->valnode->type == QI_OPR);

		memcpy(state->curitem, in->valnode, sizeof(QueryOperator));

		Assert(in->nchild <= 2);
		state->curitem++;

		fillQT(state, in->child[0]);

		if (in->nchild == 2)
		{
			curitem->qoperator.left = state->curitem - curitem;
			fillQT(state, in->child[1]);
		}
	}
}
