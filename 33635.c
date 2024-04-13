static int sn_cmp(const ASN1_OBJECT * const *a, const unsigned int *b)
	{ return(strcmp((*a)->sn,nid_objs[*b].sn)); }
