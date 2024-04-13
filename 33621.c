int OBJ_ln2nid(const char *s)
	{
	ASN1_OBJECT o;
	const ASN1_OBJECT *oo= &o;
	ADDED_OBJ ad,*adp;
	const unsigned int *op;

	o.ln=s;
	if (added != NULL)
		{
		ad.type=ADDED_LNAME;
		ad.obj= &o;
		adp=lh_ADDED_OBJ_retrieve(added,&ad);
		if (adp != NULL) return (adp->obj->nid);
		}
	op=OBJ_bsearch_ln(&oo, ln_objs, NUM_LN);
	if (op == NULL) return(NID_undef);
	return(nid_objs[*op].nid);
	}
