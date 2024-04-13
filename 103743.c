static void xhci_free_streams(XHCIEPContext *epctx)
{
    assert(epctx->pstreams != NULL);

    g_free(epctx->pstreams);
    epctx->pstreams = NULL;
    epctx->nr_pstreams = 0;
}
