addToGroup(struct rx_call *call, afs_int32 aid, afs_int32 gid, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 tempu;
    afs_int32 tempg;
    struct prentry tentry;
    struct prentry uentry;

    code = Initdb();
    if (code != PRSUCCESS)
	return code;
    if (gid == ANYUSERID || gid == AUTHUSERID)
	return PRPERM;
    if (aid == ANONYMOUSID)
	return PRPERM;
    code = ubik_BeginTrans(dbase, UBIK_WRITETRANS, &tt);
    if (code)
	return code;
    code = ubik_SetLock(tt, 1, 1, LOCKWRITE);
    if (code)
	ABORT_WITH(tt, code);
    code = read_DbHeader(tt);
    if (code)
	ABORT_WITH(tt, code);

    code = WhoIsThis(call, tt, cid);
    if (code)
	ABORT_WITH(tt, PRPERM);
    tempu = FindByID(tt, aid);
    if (!tempu)
	ABORT_WITH(tt, PRNOENT);
    memset(&uentry, 0, sizeof(uentry));
    code = pr_ReadEntry(tt, 0, tempu, &uentry);
    if (code != 0)
	ABORT_WITH(tt, code);

#if !defined(SUPERGROUPS)
    /* we don't allow groups as members of groups at present */
    if (uentry.flags & PRGRP)
	ABORT_WITH(tt, PRNOTUSER);
#endif

    tempg = FindByID(tt, gid);
    if (!tempg)
	ABORT_WITH(tt, PRNOENT);
    code = pr_ReadEntry(tt, 0, tempg, &tentry);
    if (code != 0)
	ABORT_WITH(tt, code);
    /* make sure that this is a group */
    if (!(tentry.flags & PRGRP))
	ABORT_WITH(tt, PRNOTGROUP);
    if (!AccessOK(tt, *cid, &tentry, PRP_ADD_MEM, PRP_ADD_ANY))
	ABORT_WITH(tt, PRPERM);

    code = AddToEntry(tt, &tentry, tempg, aid);
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

#if defined(SUPERGROUPS)
    if (uentry.flags & PRGRP)
	code = AddToSGEntry(tt, &uentry, tempu, gid);	/* mod group to be in sg */
    else
#endif
	/* now, modify the user's entry as well */
	code = AddToEntry(tt, &uentry, tempu, gid);
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);
    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
