ProcRenderQueryVersion (ClientPtr client)
{
    RenderClientPtr pRenderClient = GetRenderClient (client);
    xRenderQueryVersionReply rep;
    register int n;
    REQUEST(xRenderQueryVersionReq);

    pRenderClient->major_version = stuff->majorVersion;
    pRenderClient->minor_version = stuff->minorVersion;

    REQUEST_SIZE_MATCH(xRenderQueryVersionReq);
    memset(&rep, 0, sizeof(xRenderQueryVersionReply));
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;

    if ((stuff->majorVersion * 1000 + stuff->minorVersion) <
        (SERVER_RENDER_MAJOR_VERSION * 1000 + SERVER_RENDER_MINOR_VERSION))
    {
	rep.majorVersion = stuff->majorVersion;
	rep.minorVersion = stuff->minorVersion;
    } else
    {
	rep.majorVersion = SERVER_RENDER_MAJOR_VERSION;
	rep.minorVersion = SERVER_RENDER_MINOR_VERSION;
    }

    if (client->swapped) {
    	swaps(&rep.sequenceNumber, n);
    	swapl(&rep.length, n);
	swapl(&rep.majorVersion, n);
	swapl(&rep.minorVersion, n);
    }
    WriteToClient(client, sizeof(xRenderQueryVersionReply), (char *)&rep);
    return Success;
}
