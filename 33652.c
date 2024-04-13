SPR_RemoveFromGroup(struct rx_call *call, afs_int32 aid, afs_int32 gid)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = removeFromGroup(call, aid, gid, &cid);
    osi_auditU(call, PTS_RmFmGrpEvent, code, AUD_ID, gid, AUD_ID, aid,
	       AUD_END);
    ViceLog(5, ("PTS_RemoveFromGroup: code %d cid %d gid %d aid %d\n", code, cid, gid, aid));
    return code;
}
