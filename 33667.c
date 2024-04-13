listEntry(struct rx_call *call, afs_int32 aid, struct prcheckentry *aentry,
	  afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 temp;
    struct prentry tentry;

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
    if (code != 0)
	ABORT_WITH(tt, code);
    if (!AccessOK(tt, *cid, &tentry, PRP_STATUS_MEM, PRP_STATUS_ANY))
	ABORT_WITH(tt, PRPERM);

    aentry->flags = tentry.flags >> PRIVATE_SHIFT;
    if (aentry->flags == 0) {
	if (tentry.flags & PRGRP)
	    aentry->flags = prp_group_default >> PRIVATE_SHIFT;
	else
	    aentry->flags = prp_user_default >> PRIVATE_SHIFT;
    }
    aentry->owner = tentry.owner;
    aentry->id = tentry.id;
    strncpy(aentry->name, tentry.name, PR_MAXNAMELEN);
    aentry->creator = tentry.creator;
    aentry->ngroups = tentry.ngroups;
    aentry->nusers = tentry.nusers;
    aentry->count = tentry.count;
    memset(aentry->reserved, 0, sizeof(aentry->reserved));
    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
