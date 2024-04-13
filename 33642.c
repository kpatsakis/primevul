SPR_IDToName(struct rx_call *call, idlist *aid, namelist *aname)
{
    afs_int32 code;

    code = idToName(call, aid, aname);
    osi_auditU(call, PTS_IdToNmEvent, code, AUD_END);
    ViceLog(125, ("PTS_IDToName: code %d\n", code));
    return code;
}
