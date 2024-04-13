SPR_SetMax(struct rx_call *call, afs_int32 aid, afs_int32 gflag)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = setMax(call, aid, gflag, &cid);
    osi_auditU(call, PTS_SetMaxEvent, code, AUD_ID, aid, AUD_LONG, gflag,
	       AUD_END);
    ViceLog(125, ("PTS_SetMax: code %d cid %d aid %d gflag %d\n", code, cid, aid, gflag));
    return code;
}
