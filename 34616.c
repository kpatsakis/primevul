ProcGetFontPath(ClientPtr client)
{
    xGetFontPathReply reply;
    int rc, stringLens, numpaths;
    unsigned char *bufferStart;

    /* REQUEST (xReq); */

    REQUEST_SIZE_MATCH(xReq);
    rc = GetFontPath(client, &numpaths, &stringLens, &bufferStart);
    if (rc != Success)
        return rc;

    reply = (xGetFontPathReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(stringLens + numpaths),
        .nPaths = numpaths
    };

    WriteReplyToClient(client, sizeof(xGetFontPathReply), &reply);
    if (stringLens || numpaths)
        WriteToClient(client, stringLens + numpaths, bufferStart);
    return Success;
}
