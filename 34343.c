SProcRenderCreatePicture (ClientPtr client)
{
    register int n;
    REQUEST(xRenderCreatePictureReq);
    swaps(&stuff->length, n);
    swapl(&stuff->pid, n);
    swapl(&stuff->drawable, n);
    swapl(&stuff->format, n);
    swapl(&stuff->mask, n);
    SwapRestL(stuff);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
