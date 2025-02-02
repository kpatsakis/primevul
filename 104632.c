pvscsi_dbg_dump_tx_rings_config(PVSCSICmdDescSetupRings *rc)
{
    int i;
    trace_pvscsi_tx_rings_ppn("Rings State", rc->ringsStatePPN);

    trace_pvscsi_tx_rings_num_pages("Request Ring", rc->reqRingNumPages);
    for (i = 0; i < rc->reqRingNumPages; i++) {
        trace_pvscsi_tx_rings_ppn("Request Ring", rc->reqRingPPNs[i]);
    }

    trace_pvscsi_tx_rings_num_pages("Confirm Ring", rc->cmpRingNumPages);
    for (i = 0; i < rc->cmpRingNumPages; i++) {
        trace_pvscsi_tx_rings_ppn("Confirm Ring", rc->cmpRingPPNs[i]);
    }
}
