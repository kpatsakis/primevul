query_has_required_values(QUERYTYPE *query)
{
	if (query->size <= 0)
		return false;
	return contains_required_value(GETQUERY(query) + query->size - 1);
}
