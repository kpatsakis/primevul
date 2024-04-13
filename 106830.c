QTNEq(QTNode *a, QTNode *b)
{
	uint32		sign = a->sign & b->sign;

	if (!(sign == a->sign && sign == b->sign))
		return 0;

	return (QTNodeCompare(a, b) == 0) ? true : false;
}
