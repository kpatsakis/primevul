SPR_GetCPS2(struct rx_call *call, afs_int32 aid, afs_int32 ahost,
	    prlist *alist, afs_int32 *over)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = getCPS2(call, aid, ahost, alist, over, &cid);
    osi_auditU(call, PTS_GetCPS2Event, code, AUD_ID, aid, AUD_HOST, htonl(ahost),
	       AUD_END);
    ViceLog(125, ("PTS_GetCPS2: code %d cid %d aid %d ahost %d\n", code, cid, aid, ahost));
    return code;
}
