SProcRenderFreePicture (ClientPtr client)
{
    register int n;
    REQUEST(xRenderFreePictureReq);
    swaps(&stuff->length, n);
    swapl(&stuff->picture, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
