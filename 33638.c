SPR_Delete(struct rx_call *call, afs_int32 aid)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = Delete(call, aid, &cid);
    osi_auditU(call, PTS_DelEvent, code, AUD_ID, aid, AUD_END);
    ViceLog(5, ("PTS_Delete: code %d cid %d aid %d\n", code, cid, aid));
    return code;
}
