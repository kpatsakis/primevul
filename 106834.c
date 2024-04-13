QTNodeCompare(QTNode *an, QTNode *bn)
{
	/* since this function recurses, it could be driven to stack overflow. */
	check_stack_depth();

	if (an->valnode->type != bn->valnode->type)
		return (an->valnode->type > bn->valnode->type) ? -1 : 1;

	if (an->valnode->type == QI_OPR)
	{
		QueryOperator *ao = &an->valnode->qoperator;
		QueryOperator *bo = &bn->valnode->qoperator;

		if (ao->oper != bo->oper)
			return (ao->oper > bo->oper) ? -1 : 1;

		if (an->nchild != bn->nchild)
			return (an->nchild > bn->nchild) ? -1 : 1;

		{
			int			i,
						res;

			for (i = 0; i < an->nchild; i++)
				if ((res = QTNodeCompare(an->child[i], bn->child[i])) != 0)
					return res;
		}
		return 0;
	}
	else if (an->valnode->type == QI_VAL)
	{
		QueryOperand *ao = &an->valnode->qoperand;
		QueryOperand *bo = &bn->valnode->qoperand;

		if (ao->valcrc != bo->valcrc)
		{
			return (ao->valcrc > bo->valcrc) ? -1 : 1;
		}

		return tsCompareString(an->word, ao->length, bn->word, bo->length, false);
	}
	else
	{
		elog(ERROR, "unrecognized QueryItem type: %d", an->valnode->type);
		return 0;				/* keep compiler quiet */
	}
}
