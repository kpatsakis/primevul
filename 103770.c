static void xhci_reset_streams(XHCIEPContext *epctx)
{
    unsigned int i;

    for (i = 0; i < epctx->nr_pstreams; i++) {
        epctx->pstreams[i].sct = -1;
    }
}
