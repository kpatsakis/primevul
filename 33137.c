static int nid_cb(const char *elem, int len, void *arg)
	{
	nid_cb_st *narg = arg;
	size_t i;
	int nid;
	char etmp[20];
	if (narg->nidcnt == MAX_CURVELIST)
		return 0;
	if (len > (int)(sizeof(etmp) - 1))
		return 0;
	memcpy(etmp, elem, len);
	etmp[len] = 0;
	nid = EC_curve_nist2nid(etmp);
	if (nid == NID_undef)
		nid = OBJ_sn2nid(etmp);
	if (nid == NID_undef)
		nid = OBJ_ln2nid(etmp);
	if (nid == NID_undef)
		return 0;
	for (i = 0; i < narg->nidcnt; i++)
		if (narg->nid_arr[i] == nid)
			return 0;
	narg->nid_arr[narg->nidcnt++] = nid;
	return 1;
	}
