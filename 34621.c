ProcGrabServer(ClientPtr client)
{
    int rc;

    REQUEST_SIZE_MATCH(xReq);
    if (grabState != GrabNone && client != grabClient) {
        ResetCurrentRequest(client);
        client->sequence--;
        BITSET(grabWaiters, client->index);
        IgnoreClient(client);
        return Success;
    }
    rc = OnlyListenToOneClient(client);
    if (rc != Success)
        return rc;
    grabState = GrabKickout;
    grabClient = client;

    if (ServerGrabCallback) {
        ServerGrabInfoRec grabinfo;

        grabinfo.client = client;
        grabinfo.grabstate = SERVER_GRABBED;
        CallCallbacks(&ServerGrabCallback, (void *) &grabinfo);
    }

    return Success;
}
