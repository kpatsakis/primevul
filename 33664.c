idToName(struct rx_call *call, idlist *aid, namelist *aname)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 i;
    int size;
    int count = 0;

    /* leave this first for rpc stub */
    size = aid->idlist_len;
    if (size == 0)
	return 0;
    if (size < 0 || size > INT_MAX / PR_MAXNAMELEN)
	return PRTOOMANY;
    aname->namelist_val = (prname *) malloc(size * PR_MAXNAMELEN);
    aname->namelist_len = 0;
    if (aname->namelist_val == 0)
	return PRNOMEM;
    if (aid->idlist_len == 0)
	return 0;
    if (size == 0)
	return PRTOOMANY;	/* rxgen will probably handle this */

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

    for (i = 0; i < aid->idlist_len; i++) {
	code = IDToName(tt, aid->idlist_val[i], aname->namelist_val[i]);
	if (code != PRSUCCESS)
	    sprintf(aname->namelist_val[i], "%d", aid->idlist_val[i]);
        osi_audit(PTS_IdToNmEvent, code, AUD_ID, aid->idlist_val[i],
		  AUD_STR, aname->namelist_val[i], AUD_END);
	ViceLog(125, ("PTS_idToName: code %d aid %d aname %s\n", code,
		      aid->idlist_val[i], aname->namelist_val[i]));
	if (count++ > 50) {
#ifndef AFS_PTHREAD_ENV
	    IOMGR_Poll();
#endif
	    count = 0;
	}
    }
    aname->namelist_len = aid->idlist_len;

    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
