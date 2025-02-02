static void lsi_do_msgin(LSIState *s)
{
    uint8_t len;
    trace_lsi_do_msgin(s->dbc, s->msg_len);
    s->sfbr = s->msg[0];
    len = s->msg_len;
    assert(len > 0 && len <= LSI_MAX_MSGIN_LEN);
    if (len > s->dbc)
        len = s->dbc;
    pci_dma_write(PCI_DEVICE(s), s->dnad, s->msg, len);
    /* Linux drivers rely on the last byte being in the SIDL.  */
    s->sidl = s->msg[len - 1];
    s->msg_len -= len;
    if (s->msg_len) {
        memmove(s->msg, s->msg + len, s->msg_len);
    } else {
        /* ??? Check if ATN (not yet implemented) is asserted and maybe
           switch to PHASE_MO.  */
        switch (s->msg_action) {
        case LSI_MSG_ACTION_COMMAND:
            lsi_set_phase(s, PHASE_CMD);
            break;
        case LSI_MSG_ACTION_DISCONNECT:
            lsi_disconnect(s);
            break;
        case LSI_MSG_ACTION_DOUT:
            lsi_set_phase(s, PHASE_DO);
            break;
        case LSI_MSG_ACTION_DIN:
            lsi_set_phase(s, PHASE_DI);
            break;
        default:
            abort();
        }
    }
}
