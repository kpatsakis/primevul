cmpQTN(const void *a, const void *b)
{
	return QTNodeCompare(*(QTNode *const *) a, *(QTNode *const *) b);
}
