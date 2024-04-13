signconsistent(QUERYTYPE *query, BITVEC sign, bool calcnot)
{
	return execute(GETQUERY(query) + query->size - 1,
				   (void *) sign, calcnot,
				   checkcondition_bit);
}
