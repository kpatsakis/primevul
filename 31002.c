ReleaseActiveGrabs(ClientPtr client)
{
    DeviceIntPtr dev;
    Bool done;

    /* XXX CloseDownClient should remove passive grabs before
     * releasing active grabs.
     */
    do {
        done = TRUE;
        for (dev = inputInfo.devices; dev; dev = dev->next) {
            if (dev->deviceGrab.grab &&
                SameClient(dev->deviceGrab.grab, client)) {
                (*dev->deviceGrab.DeactivateGrab) (dev);
                done = FALSE;
            }
        }
    } while (!done);
}
