setMax(struct rx_call *call, afs_int32 aid, afs_int32 gflag, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;

    code = Initdb();
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
    if (!AccessOK(tt, *cid, 0, 0, 0))
	ABORT_WITH(tt, PRPERM);
    if (((gflag & PRGRP) && (aid > 0)) || (!(gflag & PRGRP) && (aid < 0)))
	ABORT_WITH(tt, PRBADARG);

    code = SetMax(tt, aid, gflag);
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
