PickPointer(ClientPtr client)
{
    DeviceIntPtr it = inputInfo.devices;

    /* First, check if the client currently has a grab on a device. Even
     * keyboards count. */
    for (it = inputInfo.devices; it; it = it->next) {
        GrabPtr grab = it->deviceGrab.grab;

        if (grab && grab->grabtype == CORE && SameClient(grab, client)) {
            it = GetMaster(it, MASTER_POINTER);
            return it;          /* Always return a core grabbed device */
        }
    }

    if (!client->clientPtr) {
        it = inputInfo.devices;
        while (it) {
            if (IsMaster(it) && it->spriteInfo->spriteOwner) {
                client->clientPtr = it;
                break;
            }
            it = it->next;
        }
    }
    return client->clientPtr;
}
