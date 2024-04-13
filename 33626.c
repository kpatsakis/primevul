int OBJ_obj2nid(const ASN1_OBJECT *a)
	{
	const unsigned int *op;
	ADDED_OBJ ad,*adp;

	if (a == NULL)
		return(NID_undef);
	if (a->nid != 0)
		return(a->nid);

	if (added != NULL)
		{
		ad.type=ADDED_DATA;
		ad.obj=(ASN1_OBJECT *)a; /* XXX: ugly but harmless */
		adp=lh_ADDED_OBJ_retrieve(added,&ad);
		if (adp != NULL) return (adp->obj->nid);
		}
	op=OBJ_bsearch_obj(&a, obj_objs, NUM_OBJ);
	if (op == NULL)
		return(NID_undef);
	return(nid_objs[*op].nid);
	}
