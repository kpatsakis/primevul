UpdateEntry(struct rx_call *call, afs_int32 aid, char *name,
	    struct PrUpdateEntry *uentry, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    struct prentry tentry;
    afs_int32 loc;
    int id = 0;

    code = Initdb();
    if (code)
	return code;
    if (code != PRSUCCESS)
	return code;
    if (aid) {
	id = aid;
	if (aid == SYSADMINID || aid == ANYUSERID || aid == AUTHUSERID
	    || aid == ANONYMOUSID)
	    return PRPERM;
    }
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
    code = IsAMemberOf(tt, *cid, SYSADMINID);
    if (!code && !pr_noAuth)
	ABORT_WITH(tt, PRPERM);

    /* Read in entry to be deleted */
    if (id) {
	loc = FindByID(tt, aid);
    } else {
	loc = FindByName(tt, name, &tentry);
    }
    if (loc == 0)
	ABORT_WITH(tt, PRNOENT);
    code = pr_ReadEntry(tt, 0, loc, &tentry);
    if (code)
	ABORT_WITH(tt, PRDBFAIL);

    if (uentry->Mask & PRUPDATE_NAMEHASH) {
	int tloc;
	code = RemoveFromNameHash(tt, tentry.name, &tloc);
	if (code != PRSUCCESS)
	    ABORT_WITH(tt, PRDBFAIL);
	code = AddToNameHash(tt, tentry.name, loc);
	if (code)
	    ABORT_WITH(tt, code);
    }

    if (uentry->Mask & PRUPDATE_IDHASH) {
	int tloc;
	if (!id)
	    id = tentry.id;
	code = RemoveFromIDHash(tt, id, &tloc);
	if (code != PRSUCCESS)
	    ABORT_WITH(tt, PRDBFAIL);
	code = AddToIDHash(tt, id, loc);
	if (code)
	    ABORT_WITH(tt, code);
    }

    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
