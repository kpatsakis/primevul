ProcListHosts(ClientPtr client)
{
    xListHostsReply reply;
    int len, nHosts, result;
    BOOL enabled;
    void *pdata;

    /* REQUEST(xListHostsReq); */

    REQUEST_SIZE_MATCH(xListHostsReq);

    /* untrusted clients can't list hosts */
    result = XaceHook(XACE_SERVER_ACCESS, client, DixReadAccess);
    if (result != Success)
        return result;

    result = GetHosts(&pdata, &nHosts, &len, &enabled);
    if (result != Success)
        return result;

    reply = (xListHostsReply) {
        .type = X_Reply,
        .enabled = enabled,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(len),
        .nHosts = nHosts
    };
    WriteReplyToClient(client, sizeof(xListHostsReply), &reply);
    if (nHosts) {
        client->pSwapReplyFunc = (ReplySwapPtr) SLHostsExtend;
        WriteSwappedDataToClient(client, len, pdata);
    }
    free(pdata);
    return Success;
}
