SProcRenderQueryFilters (ClientPtr client)
{
    register int n;
    REQUEST (xRenderQueryFiltersReq);
    REQUEST_SIZE_MATCH (xRenderQueryFiltersReq);

    swaps(&stuff->length, n);
    swapl(&stuff->drawable, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
