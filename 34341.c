SProcRenderCreateGlyphSet (ClientPtr client)
{
    register int n;
    REQUEST(xRenderCreateGlyphSetReq);
    swaps(&stuff->length, n);
    swapl(&stuff->gsid, n);
    swapl(&stuff->format, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
