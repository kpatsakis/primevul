SProcRenderReferenceGlyphSet (ClientPtr client)
{
    register int n;
    REQUEST(xRenderReferenceGlyphSetReq);
    swaps(&stuff->length, n);
    swapl(&stuff->gsid, n);
    swapl(&stuff->existing, n);
    return (*ProcRenderVector[stuff->renderReqType])  (client);
}
