setFieldsEntry(struct rx_call *call,
	       afs_int32 id,
	       afs_int32 mask, /* specify which fields to update */
	       afs_int32 flags, afs_int32 ngroups, afs_int32 nusers,
	       afs_int32 spare1, afs_int32 spare2, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 pos;
    struct prentry tentry;
    afs_int32 tflags;

    if (mask == 0)
	return 0;		/* no-op */
    code = Initdb();
    if (code)
	return code;
    if (id == ANYUSERID || id == AUTHUSERID || id == ANONYMOUSID)
	return PRPERM;
    if (code != PRSUCCESS)
	return code;
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
    pos = FindByID(tt, id);
    if (!pos)
	ABORT_WITH(tt, PRNOENT);
    code = pr_ReadEntry(tt, 0, pos, &tentry);
    if (code)
	ABORT_WITH(tt, code);
    tflags = tentry.flags;

    if (mask & (PR_SF_NGROUPS | PR_SF_NUSERS)) {
	if (!AccessOK(tt, *cid, 0, 0, 0))
	    ABORT_WITH(tt, PRPERM);
	if ((tflags & PRQUOTA) == 0) {	/* default if only setting one */
	    tentry.ngroups = tentry.nusers = 20;
	}
    } else {
	if (!AccessOK(tt, *cid, &tentry, 0, 0))
	    ABORT_WITH(tt, PRPERM);
    }

    if (mask & 0xffff) {	/* if setting flag bits */
	afs_int32 flagsMask = mask & 0xffff;
	tflags &= ~(flagsMask << PRIVATE_SHIFT);
	tflags |= (flags & flagsMask) << PRIVATE_SHIFT;
	tflags |= PRACCESS;
    }

    if (mask & PR_SF_NGROUPS) {	/* setting group limit */
	if (ngroups < 0)
	    ABORT_WITH(tt, PRBADARG);
	tentry.ngroups = ngroups;
	tflags |= PRQUOTA;
    }

    if (mask & PR_SF_NUSERS) {	/* setting foreign user limit */
	if (nusers < 0)
	    ABORT_WITH(tt, PRBADARG);
	tentry.nusers = nusers;
	tflags |= PRQUOTA;
    }
    tentry.flags = tflags;

    code = pr_WriteEntry(tt, 0, pos, &tentry);
    if (code)
	ABORT_WITH(tt, code);

    code = ubik_EndTrans(tt);
    return code;
}
