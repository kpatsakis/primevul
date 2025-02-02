pvscsi_ring_pop_req_descr(PVSCSIRingInfo *mgr)
{
    uint32_t ready_ptr = RS_GET_FIELD(mgr, reqProdIdx);

    if (ready_ptr != mgr->consumed_ptr) {
        uint32_t next_ready_ptr =
            mgr->consumed_ptr++ & mgr->txr_len_mask;
        uint32_t next_ready_page =
            next_ready_ptr / PVSCSI_MAX_NUM_REQ_ENTRIES_PER_PAGE;
        uint32_t inpage_idx =
            next_ready_ptr % PVSCSI_MAX_NUM_REQ_ENTRIES_PER_PAGE;

        return mgr->req_ring_pages_pa[next_ready_page] +
               inpage_idx * sizeof(PVSCSIRingReqDesc);
    } else {
        return 0;
    }
}
