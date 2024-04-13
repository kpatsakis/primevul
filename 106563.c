contains_required_value(ITEM *curitem)
{
	/* since this function recurses, it could be driven to stack overflow */
	check_stack_depth();

	if (curitem->type == VAL)
		return true;
	else if (curitem->val == (int32) '!')
	{
		/*
		 * Assume anything under a NOT is non-required.  For some cases with
		 * nested NOTs, we could prove there's a required value, but it seems
		 * unlikely to be worth the trouble.
		 */
		return false;
	}
	else if (curitem->val == (int32) '&')
	{
		/* If either side has a required value, we're good */
		if (contains_required_value(curitem + curitem->left))
			return true;
		else
			return contains_required_value(curitem - 1);
	}
	else
	{							/* |-operator */
		/* Both sides must have required values */
		if (contains_required_value(curitem + curitem->left))
			return contains_required_value(curitem - 1);
		else
			return false;
	}
}
