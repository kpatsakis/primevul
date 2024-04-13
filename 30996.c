ProcUngrabKeyboard(ClientPtr client)
{
    DeviceIntPtr device = PickKeyboard(client);
    GrabPtr grab;
    TimeStamp time;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    UpdateCurrentTime();

    grab = device->deviceGrab.grab;

    time = ClientTimeToServerTime(stuff->id);
    if ((CompareTimeStamps(time, currentTime) != LATER) &&
        (CompareTimeStamps(time, device->deviceGrab.grabTime) != EARLIER) &&
        (grab) && SameClient(grab, client) && grab->grabtype == CORE)
        (*device->deviceGrab.DeactivateGrab) (device);
    return Success;
}
