pvscsi_on_cmd_setup_rings(PVSCSIState *s)
{
    PVSCSICmdDescSetupRings *rc =
        (PVSCSICmdDescSetupRings *) s->curr_cmd_data;

    trace_pvscsi_on_cmd_arrived("PVSCSI_CMD_SETUP_RINGS");

    if (!rc->reqRingNumPages
        || rc->reqRingNumPages > PVSCSI_SETUP_RINGS_MAX_NUM_PAGES
        || !rc->cmpRingNumPages
        || rc->cmpRingNumPages > PVSCSI_SETUP_RINGS_MAX_NUM_PAGES) {
        return PVSCSI_COMMAND_PROCESSING_FAILED;
    }

    pvscsi_dbg_dump_tx_rings_config(rc);
    pvscsi_ring_init_data(&s->rings, rc);

    s->rings_info_valid = TRUE;
    return PVSCSI_COMMAND_PROCESSING_SUCCEEDED;
}
