ProcGrabKeyboard(ClientPtr client)
{
    xGrabKeyboardReply rep;
    BYTE status;

    REQUEST(xGrabKeyboardReq);
    int result;
    DeviceIntPtr keyboard = PickKeyboard(client);
    GrabMask mask;

    REQUEST_SIZE_MATCH(xGrabKeyboardReq);
    UpdateCurrentTime();

    mask.core = KeyPressMask | KeyReleaseMask;

    result = GrabDevice(client, keyboard, stuff->pointerMode,
                        stuff->keyboardMode, stuff->grabWindow,
                        stuff->ownerEvents, stuff->time, &mask, CORE, None,
                        None, &status);

    if (result != Success)
        return result;

    rep = (xGrabKeyboardReply) {
        .type = X_Reply,
        .status = status,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    WriteReplyToClient(client, sizeof(xGrabKeyboardReply), &rep);
    return Success;
}
