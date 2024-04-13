SProcRenderTrapezoids (ClientPtr client)
{
    register int n;
    REQUEST(xRenderTrapezoidsReq);

    REQUEST_AT_LEAST_SIZE(xRenderTrapezoidsReq);
    swaps (&stuff->length, n);
    swapl (&stuff->src, n);
    swapl (&stuff->dst, n);
    swapl (&stuff->maskFormat, n);
    swaps (&stuff->xSrc, n);
    swaps (&stuff->ySrc, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
