ProcInitialConnection(ClientPtr client)
{
    REQUEST(xReq);
    xConnClientPrefix *prefix;
    int whichbyte = 1;
    char order;

    prefix = (xConnClientPrefix *) ((char *)stuff + sz_xReq);
    order = prefix->byteOrder;
    if (order != 'l' && order != 'B' && order != 'r' && order != 'R')
	return client->noClientException = -1;
    if (((*(char *) &whichbyte) && (order == 'B' || order == 'R')) ||
	(!(*(char *) &whichbyte) && (order == 'l' || order == 'r'))) {
	client->swapped = TRUE;
	SwapConnClientPrefix(prefix);
    }
    stuff->reqType = 2;
    stuff->length += bytes_to_int32(prefix->nbytesAuthProto) +
        bytes_to_int32(prefix->nbytesAuthString);
    if (client->swapped) {
        swaps(&stuff->length);
    }
    if (order == 'r' || order == 'R') {
	client->local = FALSE;
    }
    ResetCurrentRequest(client);
    return Success;
}
