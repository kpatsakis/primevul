SProcConvertSelection(ClientPtr client)
{
    REQUEST(xConvertSelectionReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xConvertSelectionReq);
    swapl(&stuff->requestor);
    swapl(&stuff->selection);
    swapl(&stuff->target);
    swapl(&stuff->property);
    swapl(&stuff->time);
    return ((*ProcVector[X_ConvertSelection]) (client));
}
