static int obj_cmp(const ASN1_OBJECT * const *ap, const unsigned int *bp)
	{
	int j;
	const ASN1_OBJECT *a= *ap;
	const ASN1_OBJECT *b= &nid_objs[*bp];

	j=(a->length - b->length);
        if (j) return(j);
	return(memcmp(a->data,b->data,a->length));
	}
