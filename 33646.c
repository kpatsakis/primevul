SPR_ListEntries(struct rx_call *call, afs_int32 flag, afs_int32 startindex,
		prentries *bulkentries, afs_int32 *nextstartindex)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = listEntries(call, flag, startindex, bulkentries, nextstartindex, &cid);
    osi_auditU(call, PTS_LstEntsEvent, code, AUD_LONG, flag, AUD_END);
    ViceLog(125, ("PTS_ListEntries: code %d cid %d flag %d\n", code, cid, flag));
    return code;
}
