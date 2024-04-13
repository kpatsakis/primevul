QTNClearFlags(QTNode *in, uint32 flags)
{
	/* since this function recurses, it could be driven to stack overflow. */
	check_stack_depth();

	in->flags &= ~flags;

	if (in->valnode->type != QI_VAL)
	{
		int			i;

		for (i = 0; i < in->nchild; i++)
			QTNClearFlags(in->child[i], flags);
	}
}
