ProcGetScreenSaver(ClientPtr client)
{
    xGetScreenSaverReply rep;
    int rc, i;

    REQUEST_SIZE_MATCH(xReq);

    for (i = 0; i < screenInfo.numScreens; i++) {
        rc = XaceHook(XACE_SCREENSAVER_ACCESS, client, screenInfo.screens[i],
                      DixGetAttrAccess);
        if (rc != Success)
            return rc;
    }

    rep = (xGetScreenSaverReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .timeout = ScreenSaverTime / MILLI_PER_SECOND,
        .interval = ScreenSaverInterval / MILLI_PER_SECOND,
        .preferBlanking = ScreenSaverBlanking,
        .allowExposures = ScreenSaverAllowExposures
    };
    WriteReplyToClient(client, sizeof(xGetScreenSaverReply), &rep);
    return Success;
}
