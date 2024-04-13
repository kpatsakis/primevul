static void lsi_do_status(LSIState *s)
{
    uint8_t status;
    trace_lsi_do_status(s->dbc, s->status);
    if (s->dbc != 1) {
        trace_lsi_do_status_error();
    }
    s->dbc = 1;
    status = s->status;
    s->sfbr = status;
    pci_dma_write(PCI_DEVICE(s), s->dnad, &status, 1);
    lsi_set_phase(s, PHASE_MI);
    s->msg_action = LSI_MSG_ACTION_DISCONNECT;
    lsi_add_msg_byte(s, 0); /* COMMAND COMPLETE */
}
