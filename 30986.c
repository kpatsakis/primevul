ProcAllowEvents(ClientPtr client)
{
    TimeStamp time;
    DeviceIntPtr mouse = NULL;
    DeviceIntPtr keybd = NULL;

    REQUEST(xAllowEventsReq);

    REQUEST_SIZE_MATCH(xAllowEventsReq);
    UpdateCurrentTime();
    time = ClientTimeToServerTime(stuff->time);

    mouse = PickPointer(client);
    keybd = PickKeyboard(client);

    switch (stuff->mode) {
    case ReplayPointer:
        AllowSome(client, time, mouse, NOT_GRABBED);
        break;
    case SyncPointer:
        AllowSome(client, time, mouse, FREEZE_NEXT_EVENT);
        break;
    case AsyncPointer:
        AllowSome(client, time, mouse, THAWED);
        break;
    case ReplayKeyboard:
        AllowSome(client, time, keybd, NOT_GRABBED);
        break;
    case SyncKeyboard:
        AllowSome(client, time, keybd, FREEZE_NEXT_EVENT);
        break;
    case AsyncKeyboard:
        AllowSome(client, time, keybd, THAWED);
        break;
    case SyncBoth:
        AllowSome(client, time, keybd, FREEZE_BOTH_NEXT_EVENT);
        break;
    case AsyncBoth:
        AllowSome(client, time, keybd, THAWED_BOTH);
        break;
    default:
        client->errorValue = stuff->mode;
        return BadValue;
    }
    return Success;
}
