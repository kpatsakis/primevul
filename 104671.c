pvscsi_ring_flush_cmp(PVSCSIRingInfo *mgr)
{
    /* Flush descriptor changes */
    smp_wmb();

    trace_pvscsi_ring_flush_cmp(mgr->filled_cmp_ptr);

    RS_SET_FIELD(mgr, cmpProdIdx, mgr->filled_cmp_ptr);
}
