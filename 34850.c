NextAvailableClient(void *ospriv)
{
    int i;
    ClientPtr client;
    xReq data;

    i = nextFreeClientID;
    if (i == LimitClients)
        return (ClientPtr) NULL;
    clients[i] = client =
        dixAllocateObjectWithPrivates(ClientRec, PRIVATE_CLIENT);
    if (!client)
        return (ClientPtr) NULL;
    InitClient(client, i, ospriv);
    if (!InitClientResources(client)) {
        dixFreeObjectWithPrivates(client, PRIVATE_CLIENT);
        return (ClientPtr) NULL;
    }
    data.reqType = 1;
    data.length = bytes_to_int32(sz_xReq + sz_xConnClientPrefix);
    if (!InsertFakeRequest(client, (char *) &data, sz_xReq)) {
        FreeClientResources(client);
        dixFreeObjectWithPrivates(client, PRIVATE_CLIENT);
        return (ClientPtr) NULL;
    }
    if (i == currentMaxClients)
        currentMaxClients++;
    while ((nextFreeClientID < LimitClients) && clients[nextFreeClientID])
        nextFreeClientID++;

    /* Enable client ID tracking. This must be done before
     * ClientStateCallback. */
    ReserveClientIds(client);

    if (ClientStateCallback) {
        NewClientInfoRec clientinfo;

        clientinfo.client = client;
        clientinfo.prefix = (xConnSetupPrefix *) NULL;
        clientinfo.setup = (xConnSetup *) NULL;
        CallCallbacks((&ClientStateCallback), (void *) &clientinfo);
    }
    return client;
}
