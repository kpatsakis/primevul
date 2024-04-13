SPR_IsAMemberOf(struct rx_call *call, afs_int32 uid, afs_int32 gid,
		afs_int32 *flag)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = isAMemberOf(call, uid, gid, flag, &cid);
    osi_auditU(call, PTS_IsMemOfEvent, code, AUD_LONG, uid, AUD_LONG, gid,
	       AUD_END);
    ViceLog(125, ("PTS_IsAMemberOf: code %d cid %d uid %d gid %d\n", code, cid, uid, gid));
    return code;
}
