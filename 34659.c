SendConnSetup(ClientPtr client, const char *reason)
{
    xWindowRoot *root;
    int i;
    int numScreens;
    char *lConnectionInfo;
    xConnSetupPrefix *lconnSetupPrefix;

    if (reason) {
        xConnSetupPrefix csp;

        csp.success = xFalse;
        csp.lengthReason = strlen(reason);
        csp.length = bytes_to_int32(csp.lengthReason);
        csp.majorVersion = X_PROTOCOL;
        csp.minorVersion = X_PROTOCOL_REVISION;
        if (client->swapped)
            WriteSConnSetupPrefix(client, &csp);
        else
            WriteToClient(client, sz_xConnSetupPrefix, &csp);
        WriteToClient(client, (int) csp.lengthReason, reason);
        return client->noClientException = -1;
    }

    numScreens = screenInfo.numScreens;
    lConnectionInfo = ConnectionInfo;
    lconnSetupPrefix = &connSetupPrefix;

    /* We're about to start speaking X protocol back to the client by
     * sending the connection setup info.  This means the authorization
     * step is complete, and we can count the client as an
     * authorized one.
     */
    nClients++;

    client->requestVector = client->swapped ? SwappedProcVector : ProcVector;
    client->sequence = 0;
    ((xConnSetup *) lConnectionInfo)->ridBase = client->clientAsMask;
    ((xConnSetup *) lConnectionInfo)->ridMask = RESOURCE_ID_MASK;
#ifdef MATCH_CLIENT_ENDIAN
    ((xConnSetup *) lConnectionInfo)->imageByteOrder = ClientOrder(client);
    ((xConnSetup *) lConnectionInfo)->bitmapBitOrder = ClientOrder(client);
#endif
    /* fill in the "currentInputMask" */
    root = (xWindowRoot *) (lConnectionInfo + connBlockScreenStart);
#ifdef PANORAMIX
    if (noPanoramiXExtension)
        numScreens = screenInfo.numScreens;
    else
        numScreens = ((xConnSetup *) ConnectionInfo)->numRoots;
#endif

    for (i = 0; i < numScreens; i++) {
        unsigned int j;
        xDepth *pDepth;
        WindowPtr pRoot = screenInfo.screens[i]->root;

        root->currentInputMask = pRoot->eventMask | wOtherEventMasks(pRoot);
        pDepth = (xDepth *) (root + 1);
        for (j = 0; j < root->nDepths; j++) {
            pDepth = (xDepth *) (((char *) (pDepth + 1)) +
                                 pDepth->nVisuals * sizeof(xVisualType));
        }
        root = (xWindowRoot *) pDepth;
    }

    if (client->swapped) {
        WriteSConnSetupPrefix(client, lconnSetupPrefix);
        WriteSConnectionInfo(client,
                             (unsigned long) (lconnSetupPrefix->length << 2),
                             lConnectionInfo);
    }
    else {
        WriteToClient(client, sizeof(xConnSetupPrefix), lconnSetupPrefix);
        WriteToClient(client, (int) (lconnSetupPrefix->length << 2),
		      lConnectionInfo);
    }
    client->clientState = ClientStateRunning;
    if (ClientStateCallback) {
        NewClientInfoRec clientinfo;

        clientinfo.client = client;
        clientinfo.prefix = lconnSetupPrefix;
        clientinfo.setup = (xConnSetup *) lConnectionInfo;
        CallCallbacks((&ClientStateCallback), (void *) &clientinfo);
    }
    return Success;
}
