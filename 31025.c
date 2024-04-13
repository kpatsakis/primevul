SProcChangeWindowAttributes(ClientPtr client)
{
    REQUEST(xChangeWindowAttributesReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xChangeWindowAttributesReq);
    swapl(&stuff->window);
    swapl(&stuff->valueMask);
    SwapRestL(stuff);
    return ((*ProcVector[X_ChangeWindowAttributes]) (client));
}
