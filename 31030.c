SProcInternAtom(ClientPtr client)
{
    REQUEST(xInternAtomReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xInternAtomReq);
    swaps(&stuff->nbytes);
    return ((*ProcVector[X_InternAtom]) (client));
}
