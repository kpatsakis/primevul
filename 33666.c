listElements(struct rx_call *call, afs_int32 aid, prlist *alist,
	     afs_int32 *over, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 temp;
    struct prentry tentry;

    *over = 0;
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

    code = WhoIsThis(call, tt, cid);
    if (code)
	ABORT_WITH(tt, PRPERM);

    temp = FindByID(tt, aid);
    if (!temp)
	ABORT_WITH(tt, PRNOENT);
    code = pr_ReadEntry(tt, 0, temp, &tentry);
    if (code)
	ABORT_WITH(tt, code);
    if (!AccessOK(tt, *cid, &tentry, PRP_MEMBER_MEM, PRP_MEMBER_ANY))
	ABORT_WITH(tt, PRPERM);

    code = GetList(tt, &tentry, alist, 0);
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

    code = ubik_EndTrans(tt);
    return code;
}
