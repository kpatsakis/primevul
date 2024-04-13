SProcConfigureWindow(ClientPtr client)
{
    REQUEST(xConfigureWindowReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xConfigureWindowReq);
    swapl(&stuff->window);
    swaps(&stuff->mask);
    SwapRestL(stuff);
    return ((*ProcVector[X_ConfigureWindow]) (client));

}
