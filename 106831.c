QTNFree(QTNode *in)
{
	if (!in)
		return;

	/* since this function recurses, it could be driven to stack overflow. */
	check_stack_depth();

	if (in->valnode->type == QI_VAL && in->word && (in->flags & QTN_WORDFREE) != 0)
		pfree(in->word);

	if (in->child)
	{
		if (in->valnode)
		{
			if (in->valnode->type == QI_OPR && in->nchild > 0)
			{
				int			i;

				for (i = 0; i < in->nchild; i++)
					QTNFree(in->child[i]);
			}
			if (in->flags & QTN_NEEDFREE)
				pfree(in->valnode);
		}
		pfree(in->child);
	}

	pfree(in);
}
