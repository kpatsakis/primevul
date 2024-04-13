SProcRenderCreateAnimCursor (ClientPtr client)
{
    register int n;
    REQUEST (xRenderCreateAnimCursorReq);
    REQUEST_AT_LEAST_SIZE (xRenderCreateAnimCursorReq);

    swaps(&stuff->length, n);
    swapl(&stuff->cid, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
