ASN1_OBJECT *OBJ_txt2obj(const char *s, int no_name)
	{
	int nid = NID_undef;
	ASN1_OBJECT *op=NULL;
	unsigned char *buf;
	unsigned char *p;
	const unsigned char *cp;
	int i, j;

	if(!no_name) {
		if( ((nid = OBJ_sn2nid(s)) != NID_undef) ||
			((nid = OBJ_ln2nid(s)) != NID_undef) ) 
					return OBJ_nid2obj(nid);
	}

	/* Work out size of content octets */
	i=a2d_ASN1_OBJECT(NULL,0,s,-1);
	if (i <= 0) {
		/* Don't clear the error */
		/*ERR_clear_error();*/
		return NULL;
	}
	/* Work out total size */
	j = ASN1_object_size(0,i,V_ASN1_OBJECT);

	if((buf=(unsigned char *)OPENSSL_malloc(j)) == NULL) return NULL;

	p = buf;
	/* Write out tag+length */
	ASN1_put_object(&p,0,i,V_ASN1_OBJECT,V_ASN1_UNIVERSAL);
	/* Write out contents */
	a2d_ASN1_OBJECT(p,i,s,-1);

	cp=buf;
	op=d2i_ASN1_OBJECT(NULL,&cp,j);
	OPENSSL_free(buf);
	return op;
	}
