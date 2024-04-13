SProcRenderQueryVersion (ClientPtr client)
{
    register int n;
    REQUEST(xRenderQueryVersionReq);

    swaps(&stuff->length, n);
    swapl(&stuff->majorVersion, n);
    swapl(&stuff->minorVersion, n);
    return (*ProcRenderVector[stuff->renderReqType])(client);
}
