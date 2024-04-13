static int ln_cmp(const ASN1_OBJECT * const *a, const unsigned int *b)
	{ return(strcmp((*a)->ln,nid_objs[*b].ln)); }
