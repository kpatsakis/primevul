ProcSetInputFocus(ClientPtr client)
{
    DeviceIntPtr kbd = PickKeyboard(client);

    REQUEST(xSetInputFocusReq);

    REQUEST_SIZE_MATCH(xSetInputFocusReq);

    return SetInputFocus(client, kbd, stuff->focus,
                         stuff->revertTo, stuff->time, FALSE);
}
