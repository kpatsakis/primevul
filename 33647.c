SPR_ListEntry(struct rx_call *call, afs_int32 aid, struct prcheckentry *aentry)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = listEntry(call, aid, aentry, &cid);
    osi_auditU(call, PTS_LstEntEvent, code, AUD_ID, aid, AUD_END);
    ViceLog(125, ("PTS_ListEntry: code %d cid %d aid %d\n", code, cid, aid));
    return code;
}
