pvscsi_ring_msg_has_room(PVSCSIRingInfo *mgr)
{
    uint32_t prodIdx = RS_GET_FIELD(mgr, msgProdIdx);
    uint32_t consIdx = RS_GET_FIELD(mgr, msgConsIdx);

    return (prodIdx - consIdx) < (mgr->msg_len_mask + 1);
}
