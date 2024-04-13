pvscsi_write_sense(PVSCSIRequest *r, uint8_t *sense, int len)
{
    r->cmp.senseLen = MIN(r->req.senseLen, len);
    r->sense_key = sense[(sense[0] & 2) ? 1 : 2];
    cpu_physical_memory_write(r->req.senseAddr, sense, r->cmp.senseLen);
}
