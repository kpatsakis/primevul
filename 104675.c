pvscsi_ring_init_msg(PVSCSIRingInfo *m, PVSCSICmdDescSetupMsgRing *ri)
{
    int i;
    uint32_t len_log2;
    uint32_t ring_size;

    if (ri->numPages > PVSCSI_SETUP_MSG_RING_MAX_NUM_PAGES) {
        return -1;
    }
    ring_size = ri->numPages * PVSCSI_MAX_NUM_MSG_ENTRIES_PER_PAGE;
    len_log2 = pvscsi_log2(ring_size - 1);

    m->msg_len_mask = MASK(len_log2);

    m->filled_msg_ptr = 0;

    for (i = 0; i < ri->numPages; i++) {
        m->msg_ring_pages_pa[i] = ri->ringPPNs[i] << VMW_PAGE_SHIFT;
    }

    RS_SET_FIELD(m, msgProdIdx, 0);
    RS_SET_FIELD(m, msgConsIdx, 0);
    RS_SET_FIELD(m, msgNumEntriesLog2, len_log2);

    trace_pvscsi_ring_init_msg(len_log2);

    /* Flush ring state page changes */
    smp_wmb();

    return 0;
}
