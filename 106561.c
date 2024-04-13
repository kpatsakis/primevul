checkcondition_bit(void *checkval, ITEM *item)
{
	return GETBIT(checkval, HASHVAL(item->val));
}
