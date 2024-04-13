SProcRenderSetPictureClipRectangles (ClientPtr client)
{
    register int n;
    REQUEST(xRenderSetPictureClipRectanglesReq);
    swaps(&stuff->length, n);
    swapl(&stuff->picture, n);
    swaps(&stuff->xOrigin, n);
    swaps(&stuff->yOrigin, n);
    SwapRestS(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
