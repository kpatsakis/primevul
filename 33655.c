SPR_UpdateEntry(struct rx_call *call, afs_int32 aid, char *name,
	        struct PrUpdateEntry *uentry)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = UpdateEntry(call, aid, name, uentry, &cid);
    osi_auditU(call, PTS_UpdEntEvent, code, AUD_ID, aid, AUD_STR, name, AUD_END);
    ViceLog(5, ("PTS_UpdateEntry: code %d cid %d aid %d name %s\n", code, cid, aid, name));
    return code;
}
