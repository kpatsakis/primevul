SProcRenderTriStrip (ClientPtr client)
{
    register int n;
    REQUEST(xRenderTriStripReq);

    REQUEST_AT_LEAST_SIZE(xRenderTriStripReq);
    swaps (&stuff->length, n);
    swapl (&stuff->src, n);
    swapl (&stuff->dst, n);
    swapl (&stuff->maskFormat, n);
    swaps (&stuff->xSrc, n);
    swaps (&stuff->ySrc, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
