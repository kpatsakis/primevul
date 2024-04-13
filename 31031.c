SProcResourceReq(ClientPtr client)
{
    REQUEST(xResourceReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xResourceReq);        /* not EXACT */
    swapl(&stuff->id);
    return (*ProcVector[stuff->reqType]) (client);
}
