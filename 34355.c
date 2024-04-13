SProcRenderSetPictureFilter (ClientPtr client)
{
    register int n;
    REQUEST (xRenderSetPictureFilterReq);
    REQUEST_AT_LEAST_SIZE (xRenderSetPictureFilterReq);

    swaps(&stuff->length, n);
    swapl(&stuff->picture, n);
    swaps(&stuff->nbytes, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
