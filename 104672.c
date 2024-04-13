pvscsi_ring_flush_msg(PVSCSIRingInfo *mgr)
{
    /* Flush descriptor changes */
    smp_wmb();

    trace_pvscsi_ring_flush_msg(mgr->filled_msg_ptr);

    RS_SET_FIELD(mgr, msgProdIdx, mgr->filled_msg_ptr);
}
