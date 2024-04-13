findoprnd(QueryItem *ptr, int size)
{
	uint32		pos;

	pos = 0;
	findoprnd_recurse(ptr, &pos, size);

	if (pos != size)
		elog(ERROR, "malformed tsquery: extra nodes");
}
