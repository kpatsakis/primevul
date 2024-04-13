SProcRenderComposite (ClientPtr client)
{
    register int n;
    REQUEST(xRenderCompositeReq);
    swaps(&stuff->length, n);
    swapl(&stuff->src, n);
    swapl(&stuff->mask, n);
    swapl(&stuff->dst, n);
    swaps(&stuff->xSrc, n);
    swaps(&stuff->ySrc, n);
    swaps(&stuff->xMask, n);
    swaps(&stuff->yMask, n);
    swaps(&stuff->xDst, n);
    swaps(&stuff->yDst, n);
    swaps(&stuff->width, n);
    swaps(&stuff->height, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
