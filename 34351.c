SProcRenderQueryPictIndexValues (ClientPtr client)
{
    register int n;
    REQUEST(xRenderQueryPictIndexValuesReq);
    swaps(&stuff->length, n);
    swapl(&stuff->format, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
