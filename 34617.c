ProcGetGeometry(ClientPtr client)
{
    xGetGeometryReply rep = { .type = X_Reply };
    int status;

    if ((status = GetGeometry(client, &rep)) != Success)
        return status;

    WriteReplyToClient(client, sizeof(xGetGeometryReply), &rep);
    return Success;
}
