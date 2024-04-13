SPR_NameToID(struct rx_call *call, namelist *aname, idlist *aid)
{
    afs_int32 code;

    code = nameToID(call, aname, aid);
    osi_auditU(call, PTS_NmToIdEvent, code, AUD_END);
    ViceLog(125, ("PTS_NameToID: code %d\n", code));
    return code;
}
