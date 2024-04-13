static void cleanup1_doall(ADDED_OBJ *a)
	{
	a->obj->nid=0;
	a->obj->flags|=ASN1_OBJECT_FLAG_DYNAMIC|
	                ASN1_OBJECT_FLAG_DYNAMIC_STRINGS|
			ASN1_OBJECT_FLAG_DYNAMIC_DATA;
	}
