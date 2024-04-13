SProcRenderAddTraps (ClientPtr client)
{
    register int n;
    REQUEST (xRenderAddTrapsReq);
    REQUEST_AT_LEAST_SIZE (xRenderAddTrapsReq);

    swaps(&stuff->length, n);
    swapl(&stuff->picture, n);
    swaps(&stuff->xOff, n);
    swaps(&stuff->yOff, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
