int OBJ_add_object(const ASN1_OBJECT *obj)
	{
	ASN1_OBJECT *o;
	ADDED_OBJ *ao[4]={NULL,NULL,NULL,NULL},*aop;
	int i;

	if (added == NULL)
		if (!init_added()) return(0);
	if ((o=OBJ_dup(obj)) == NULL) goto err;
	if (!(ao[ADDED_NID]=(ADDED_OBJ *)OPENSSL_malloc(sizeof(ADDED_OBJ)))) goto err2;
	if ((o->length != 0) && (obj->data != NULL))
		if (!(ao[ADDED_DATA]=(ADDED_OBJ *)OPENSSL_malloc(sizeof(ADDED_OBJ)))) goto err2;
	if (o->sn != NULL)
		if (!(ao[ADDED_SNAME]=(ADDED_OBJ *)OPENSSL_malloc(sizeof(ADDED_OBJ)))) goto err2;
	if (o->ln != NULL)
		if (!(ao[ADDED_LNAME]=(ADDED_OBJ *)OPENSSL_malloc(sizeof(ADDED_OBJ)))) goto err2;

	for (i=ADDED_DATA; i<=ADDED_NID; i++)
		{
		if (ao[i] != NULL)
			{
			ao[i]->type=i;
			ao[i]->obj=o;
			aop=lh_ADDED_OBJ_insert(added,ao[i]);
			/* memory leak, buit should not normally matter */
			if (aop != NULL)
				OPENSSL_free(aop);
			}
		}
	o->flags&= ~(ASN1_OBJECT_FLAG_DYNAMIC|ASN1_OBJECT_FLAG_DYNAMIC_STRINGS|
			ASN1_OBJECT_FLAG_DYNAMIC_DATA);

	return(o->nid);
err2:
	OBJerr(OBJ_F_OBJ_ADD_OBJECT,ERR_R_MALLOC_FAILURE);
err:
	for (i=ADDED_DATA; i<=ADDED_NID; i++)
		if (ao[i] != NULL) OPENSSL_free(ao[i]);
	if (o != NULL) OPENSSL_free(o);
	return(NID_undef);
	}
