SProcRenderCreateCursor (ClientPtr client)
{
    register int n;
    REQUEST(xRenderCreateCursorReq);
    REQUEST_SIZE_MATCH (xRenderCreateCursorReq);
    
    swaps(&stuff->length, n);
    swapl(&stuff->cid, n);
    swapl(&stuff->src, n);
    swaps(&stuff->x, n);
    swaps(&stuff->y, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
