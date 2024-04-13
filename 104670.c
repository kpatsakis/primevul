pvscsi_ring_cleanup(PVSCSIRingInfo *mgr)
{
    mgr->rs_pa = 0;
    mgr->txr_len_mask = 0;
    mgr->rxr_len_mask = 0;
    mgr->msg_len_mask = 0;
    mgr->consumed_ptr = 0;
    mgr->filled_cmp_ptr = 0;
    mgr->filled_msg_ptr = 0;
    memset(mgr->req_ring_pages_pa, 0, sizeof(mgr->req_ring_pages_pa));
    memset(mgr->cmp_ring_pages_pa, 0, sizeof(mgr->cmp_ring_pages_pa));
    memset(mgr->msg_ring_pages_pa, 0, sizeof(mgr->msg_ring_pages_pa));
}
