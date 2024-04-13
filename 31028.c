SProcCreateWindow(ClientPtr client)
{
    REQUEST(xCreateWindowReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xCreateWindowReq);
    swapl(&stuff->wid);
    swapl(&stuff->parent);
    swaps(&stuff->x);
    swaps(&stuff->y);
    swaps(&stuff->width);
    swaps(&stuff->height);
    swaps(&stuff->borderWidth);
    swaps(&stuff->class);
    swapl(&stuff->visual);
    swapl(&stuff->mask);
    SwapRestL(stuff);
    return ((*ProcVector[X_CreateWindow]) (client));
}
