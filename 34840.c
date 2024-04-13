ProcDbeGetVersion(ClientPtr client)
{
    /* REQUEST(xDbeGetVersionReq); */
    xDbeGetVersionReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .majorVersion = DBE_MAJOR_VERSION,
        .minorVersion = DBE_MINOR_VERSION
    };

    REQUEST_SIZE_MATCH(xDbeGetVersionReq);

    if (client->swapped) {
        swaps(&rep.sequenceNumber);
    }

    WriteToClient(client, sizeof(xDbeGetVersionReply), &rep);

    return Success;

}                               /* ProcDbeGetVersion() */
