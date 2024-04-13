SendErrorToClient(ClientPtr client, unsigned majorCode, unsigned minorCode,
                  XID resId, int errorCode)
{
    xError rep = {
        .type = X_Error,
        .errorCode = errorCode,
        .resourceID = resId,
        .minorCode = minorCode,
        .majorCode = majorCode
    };

    WriteEventsToClient(client, 1, (xEvent *) &rep);
}
