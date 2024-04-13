SPR_ListElements(struct rx_call *call, afs_int32 aid, prlist *alist,
		 afs_int32 *over)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = listElements(call, aid, alist, over, &cid);
    osi_auditU(call, PTS_LstEleEvent, code, AUD_ID, aid, AUD_END);
    ViceLog(125, ("PTS_ListElements: code %d cid %d aid %d\n", code, cid, aid));
    return code;
}
