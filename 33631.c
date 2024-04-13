static void cleanup3_doall(ADDED_OBJ *a)
	{
	if (--a->obj->nid == 0)
		ASN1_OBJECT_free(a->obj);
	OPENSSL_free(a);
	}
