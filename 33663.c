getCPS2(struct rx_call *call, afs_int32 aid, afs_uint32 ahost, prlist *alist,
	afs_int32 *over, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 temp;
    struct prentry tentry;
    struct prentry host_tentry;
    afs_int32 hostid;
    int host_list = 0;
    struct in_addr iaddr;
    char hoststr[16];

    *over = 0;
    iaddr.s_addr = ntohl(ahost);
    alist->prlist_len = 0;
    alist->prlist_val = NULL;
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

    if (aid != PRBADID) {
	temp = FindByID(tt, aid);
	if (!temp)
	    ABORT_WITH(tt, PRNOENT);
	code = pr_ReadEntry(tt, 0, temp, &tentry);
	if (code)
	    ABORT_WITH(tt, code);

	/* afs does authenticate now */
	code = WhoIsThis(call, tt, cid);
	if (code
	    || !AccessOK(tt, *cid, &tentry, PRP_MEMBER_MEM, PRP_MEMBER_ANY))
	    ABORT_WITH(tt, PRPERM);
    }
    code = NameToID(tt, afs_inet_ntoa_r(iaddr.s_addr, hoststr), &hostid);
    if (code == PRSUCCESS && hostid != 0) {
	temp = FindByID(tt, hostid);
	if (temp) {
	    code = pr_ReadEntry(tt, 0, temp, &host_tentry);
	    if (code == PRSUCCESS)
		host_list = 1;
	    else
		fprintf(stderr, "pr_ReadEntry returned %d\n", code);
	} else
	    fprintf(stderr, "FindByID Failed -- Not found\n");
    }
    if (host_list)
	code = GetList2(tt, &tentry, &host_tentry, alist, 1);
    else
	code = GetList(tt, &tentry, alist, 1);
    if (!code)
	code = addWildCards(tt, alist, ntohl(ahost));
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

    code = ubik_EndTrans(tt);
    return code;
}
