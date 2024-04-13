SProcDeleteProperty(ClientPtr client)
{
    REQUEST(xDeletePropertyReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xDeletePropertyReq);
    swapl(&stuff->window);
    swapl(&stuff->property);
    return ((*ProcVector[X_DeleteProperty]) (client));

}
