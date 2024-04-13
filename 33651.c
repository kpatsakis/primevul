SPR_NewEntry(struct rx_call *call, char aname[], afs_int32 flag, afs_int32 oid,
	     afs_int32 *aid)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = newEntry(call, aname, flag, oid, aid, &cid);
    osi_auditU(call, PTS_NewEntEvent, code, AUD_ID, *aid, AUD_STR, aname,
	       AUD_ID, oid, AUD_END);
    ViceLog(5, ("PTS_NewEntry: code %d cid %d aid %d aname %s oid %d\n", code, cid, *aid, aname, oid));
    return code;
}
