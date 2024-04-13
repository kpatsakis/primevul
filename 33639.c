SPR_DumpEntry(struct rx_call *call, afs_int32 apos,
	      struct prdebugentry *aentry)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = dumpEntry(call, apos, aentry, &cid);
    osi_auditU(call, PTS_DmpEntEvent, code, AUD_LONG, apos, AUD_END);
    ViceLog(125, ("PTS_DumpEntry: code %d cid %d apos %d\n", code, cid, apos));
    return code;
}
