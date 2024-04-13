SPR_GetHostCPS(struct rx_call *call, afs_int32 ahost, prlist *alist,
	       afs_int32 *over)
{
    afs_int32 code;

    code = getHostCPS(call, ahost, alist, over);
    osi_auditU(call, PTS_GetHCPSEvent, code, AUD_HOST, htonl(ahost), AUD_END);
    ViceLog(125, ("PTS_GetHostCPS: code %d ahost %d\n", code, ahost));
    return code;
}
