SProcRenderFreeGlyphSet (ClientPtr client)
{
    register int n;
    REQUEST(xRenderFreeGlyphSetReq);
    swaps(&stuff->length, n);
    swapl(&stuff->glyphset, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
