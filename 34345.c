SProcRenderCreateSolidFill(ClientPtr client)
{
    register int n;
    REQUEST (xRenderCreateSolidFillReq);
    REQUEST_AT_LEAST_SIZE (xRenderCreateSolidFillReq);

    swaps(&stuff->length, n);
    swapl(&stuff->pid, n);
    swaps(&stuff->color.alpha, n);
    swaps(&stuff->color.red, n);
    swaps(&stuff->color.green, n);
    swaps(&stuff->color.blue, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
