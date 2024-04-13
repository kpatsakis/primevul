int OBJ_sn2nid(const char *s)
	{
	ASN1_OBJECT o;
	const ASN1_OBJECT *oo= &o;
	ADDED_OBJ ad,*adp;
	const unsigned int *op;

	o.sn=s;
	if (added != NULL)
		{
		ad.type=ADDED_SNAME;
		ad.obj= &o;
		adp=lh_ADDED_OBJ_retrieve(added,&ad);
		if (adp != NULL) return (adp->obj->nid);
		}
	op=OBJ_bsearch_sn(&oo, sn_objs, NUM_SN);
	if (op == NULL) return(NID_undef);
	return(nid_objs[*op].nid);
	}
