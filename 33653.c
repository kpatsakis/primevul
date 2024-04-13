SPR_SetFieldsEntry(struct rx_call *call,
		   afs_int32 id,
		   afs_int32 mask, /* specify which fields to update */
		   afs_int32 flags, afs_int32 ngroups, afs_int32 nusers,
		   afs_int32 spare1, afs_int32 spare2)
{
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code =
	setFieldsEntry(call, id, mask, flags, ngroups, nusers, spare1,
		       spare2, &cid);
    osi_auditU(call, PTS_SetFldEntEvent, code, AUD_ID, id, AUD_END);
    ViceLog(5, ("PTS_SetFieldsEntry: code %d cid %d id %d\n", code, cid, id));
    return code;
}
