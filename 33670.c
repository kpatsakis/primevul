nameToID(struct rx_call *call, namelist *aname, idlist *aid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 i;
    int size;
    int count = 0;

    /* Initialize return struct */
    aid->idlist_len = 0;
    aid->idlist_val = NULL;

    size = aname->namelist_len;
    if (size == 0)
	return 0;
    if (size < 0)
	return PRTOOMANY;

    aid->idlist_val = (afs_int32 *) malloc(size * sizeof(afs_int32));
    if (!aid->idlist_val)
	return PRNOMEM;

    code = Initdb();
    if (code != PRSUCCESS)
	return code;
    code = ubik_BeginTransReadAny(dbase, UBIK_READTRANS, &tt);
    if (code)
	return code;
    code = ubik_SetLock(tt, 1, 1, LOCKREAD);
    if (code)
	ABORT_WITH(tt, code);
    code = read_DbHeader(tt);
    if (code)
	ABORT_WITH(tt, code);

    for (i = 0; i < aname->namelist_len; i++) {
	char vname[256];
	char *nameinst, *cell;

	strncpy(vname, aname->namelist_val[i], sizeof(vname));
	vname[sizeof(vname)-1] ='\0';

	nameinst = vname;
	cell = strchr(vname, '@');
	if (cell) {
	    *cell = '\0';
	    cell++;
	}

	if (cell && afs_is_foreign_ticket_name(nameinst,NULL,cell,pr_realmName))
	    code = NameToID(tt, aname->namelist_val[i], &aid->idlist_val[i]);
	else
	    code = NameToID(tt, nameinst, &aid->idlist_val[i]);

	if (code != PRSUCCESS)
	    aid->idlist_val[i] = ANONYMOUSID;
        osi_audit(PTS_NmToIdEvent, code, AUD_STR,
		   aname->namelist_val[i], AUD_ID, aid->idlist_val[i],
		   AUD_END);
	ViceLog(125, ("PTS_NameToID: code %d aname %s aid %d\n", code,
		      aname->namelist_val[i], aid->idlist_val[i]));
	if (count++ > 50) {
#ifndef AFS_PTHREAD_ENV
	    IOMGR_Poll();
#endif
	    count = 0;
	}
    }
    aid->idlist_len = aname->namelist_len;

    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
