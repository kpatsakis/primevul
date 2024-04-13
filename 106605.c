box_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	BOX		   *box = (BOX *) palloc(sizeof(BOX));
	int			isopen;
	char	   *s;
	double		x,
				y;

	if ((!path_decode(FALSE, 2, str, &isopen, &s, &(box->high)))
		|| (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type box: \"%s\"", str)));

	/* reorder corners if necessary... */
	if (box->high.x < box->low.x)
	{
		x = box->high.x;
		box->high.x = box->low.x;
		box->low.x = x;
	}
	if (box->high.y < box->low.y)
	{
		y = box->high.y;
		box->high.y = box->low.y;
		box->low.y = y;
	}

	PG_RETURN_BOX_P(box);
}
