SPR_WhereIsIt(struct rx_call *call, afs_int32 aid, afs_int32 *apos)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = whereIsIt(call, aid, apos, &cid);
    osi_auditU(call, PTS_WheIsItEvent, code, AUD_ID, aid, AUD_LONG, *apos,
	       AUD_END);
    ViceLog(125, ("PTS_WhereIsIt: code %d cid %d aid %d apos %d\n", code, cid, aid, *apos));
    return code;
}
