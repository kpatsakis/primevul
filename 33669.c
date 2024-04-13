listSuperGroups(struct rx_call *call, afs_int32 aid, prlist *alist,
		afs_int32 *over, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 temp;
    struct prentry tentry;

    alist->prlist_len = 0;
    alist->prlist_val = (afs_int32 *) 0;

    code = Initdb();
    if (code != PRSUCCESS)
	goto done;
    code = ubik_BeginTransReadAny(dbase, UBIK_READTRANS, &tt);
    if (code)
	goto done;
    code = ubik_SetLock(tt, 1, 1, LOCKREAD);
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

    code = GetSGList(tt, &tentry, alist);
    *over = 0;
    if (code == PRTOOMANY)
	*over = 1;
    else if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

    code = ubik_EndTrans(tt);

  done:
    return code;
}
