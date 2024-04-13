SPR_ListMax(struct rx_call *call, afs_int32 *uid, afs_int32 *gid)
{
    afs_int32 code;

    code = listMax(call, uid, gid);
    osi_auditU(call, PTS_LstMaxEvent, code, AUD_END);
    ViceLog(125, ("PTS_ListMax: code %d\n", code));
    return code;
}
