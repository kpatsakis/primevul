ProcGetInputFocus(ClientPtr client)
{
    DeviceIntPtr kbd = PickKeyboard(client);
    xGetInputFocusReply rep;
    FocusClassPtr focus = kbd->focus;
    int rc;

    /* REQUEST(xReq); */
    REQUEST_SIZE_MATCH(xReq);

    rc = XaceHook(XACE_DEVICE_ACCESS, client, kbd, DixGetFocusAccess);
    if (rc != Success)
        return rc;

    rep = (xGetInputFocusReply) {
        .type = X_Reply,
        .length = 0,
        .sequenceNumber = client->sequence,
        .revertTo = focus->revert
    };

    if (focus->win == NoneWin)
        rep.focus = None;
    else if (focus->win == PointerRootWin)
        rep.focus = PointerRoot;
    else
        rep.focus = focus->win->drawable.id;

    WriteReplyToClient(client, sizeof(xGetInputFocusReply), &rep);
    return Success;
}
