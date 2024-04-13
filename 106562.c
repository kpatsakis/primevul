checkcondition_gin(void *checkval, ITEM *item)
{
	GinChkVal  *gcv = (GinChkVal *) checkval;

	return gcv->mapped_check[item - gcv->first];
}
