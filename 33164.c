int tls1_set_curves(unsigned char **pext, size_t *pextlen,
			int *curves, size_t ncurves)
	{
	unsigned char *clist, *p;
	size_t i;
	/* Bitmap of curves included to detect duplicates: only works
	 * while curve ids < 32 
	 */
	unsigned long dup_list = 0;
	clist = OPENSSL_malloc(ncurves * 2);
	if (!clist)
		return 0;
	for (i = 0, p = clist; i < ncurves; i++)
		{
		unsigned long idmask;
		int id;
		id = tls1_ec_nid2curve_id(curves[i]);
		idmask = 1L << id;
		if (!id || (dup_list & idmask))
			{
			OPENSSL_free(clist);
			return 0;
			}
		dup_list |= idmask;
		s2n(id, p);
		}
	if (*pext)
		OPENSSL_free(*pext);
	*pext = clist;
	*pextlen = ncurves * 2;
	return 1;
	}
