SPR_ListOwned(struct rx_call *call, afs_int32 aid, prlist *alist,
	      afs_int32 *lastP)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = listOwned(call, aid, alist, lastP, &cid);
    osi_auditU(call, PTS_LstOwnEvent, code, AUD_ID, aid, AUD_END);
    ViceLog(125, ("PTS_ListOwned: code %d cid %d aid %d\n", code, cid, aid));
    return code;
}
