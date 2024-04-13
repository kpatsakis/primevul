pvscsi_ring_pop_msg_descr(PVSCSIRingInfo *mgr)
{
    uint32_t free_msg_ptr =
        mgr->filled_msg_ptr++ & mgr->msg_len_mask;
    uint32_t free_msg_page =
        free_msg_ptr / PVSCSI_MAX_NUM_MSG_ENTRIES_PER_PAGE;
    uint32_t inpage_idx =
        free_msg_ptr % PVSCSI_MAX_NUM_MSG_ENTRIES_PER_PAGE;
    return mgr->msg_ring_pages_pa[free_msg_page] +
           inpage_idx * sizeof(PVSCSIRingMsgDesc);
}
