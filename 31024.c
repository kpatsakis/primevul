SProcChangeProperty(ClientPtr client)
{
    REQUEST(xChangePropertyReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xChangePropertyReq);
    swapl(&stuff->window);
    swapl(&stuff->property);
    swapl(&stuff->type);
    swapl(&stuff->nUnits);
    switch (stuff->format) {
    case 8:
        break;
    case 16:
        SwapRestS(stuff);
        break;
    case 32:
        SwapRestL(stuff);
        break;
    }
    return ((*ProcVector[X_ChangeProperty]) (client));
}
