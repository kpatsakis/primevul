pvscsi_ring_flush_req(PVSCSIRingInfo *mgr)
{
    RS_SET_FIELD(mgr, reqConsIdx, mgr->consumed_ptr);
}
