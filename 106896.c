varbit_transform(PG_FUNCTION_ARGS)
{
	FuncExpr   *expr = (FuncExpr *) PG_GETARG_POINTER(0);
	Node	   *ret = NULL;
	Node	   *typmod;

	Assert(IsA(expr, FuncExpr));
	Assert(list_length(expr->args) >= 2);

	typmod = (Node *) lsecond(expr->args);

	if (IsA(typmod, Const) &&!((Const *) typmod)->constisnull)
	{
		Node	   *source = (Node *) linitial(expr->args);
		int32		new_typmod = DatumGetInt32(((Const *) typmod)->constvalue);
		int32		old_max = exprTypmod(source);
		int32		new_max = new_typmod;

		/* Note: varbit() treats typmod 0 as invalid, so we do too */
		if (new_max <= 0 || (old_max > 0 && old_max <= new_max))
			ret = relabel_to_typmod(source, new_typmod);
	}

	PG_RETURN_POINTER(ret);
}
