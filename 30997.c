ProcUngrabPointer(ClientPtr client)
{
    DeviceIntPtr device = PickPointer(client);
    GrabPtr grab;
    TimeStamp time;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    UpdateCurrentTime();
    grab = device->deviceGrab.grab;

    time = ClientTimeToServerTime(stuff->id);
    if ((CompareTimeStamps(time, currentTime) != LATER) &&
        (CompareTimeStamps(time, device->deviceGrab.grabTime) != EARLIER) &&
        (grab) && SameClient(grab, client))
        (*device->deviceGrab.DeactivateGrab) (device);
    return Success;
}
