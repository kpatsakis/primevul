CoreGrabInterferes(DeviceIntPtr device, GrabPtr grab)
{
    DeviceIntPtr other;
    BOOL interfering = FALSE;

    for (other = inputInfo.devices; other; other = other->next) {
        GrabPtr othergrab = other->deviceGrab.grab;

        if (othergrab && othergrab->grabtype == CORE &&
            SameClient(grab, rClient(othergrab)) &&
            ((IsPointerDevice(grab->device) &&
              IsPointerDevice(othergrab->device)) ||
             (IsKeyboardDevice(grab->device) &&
              IsKeyboardDevice(othergrab->device)))) {
            interfering = TRUE;
            break;
        }
    }

    return interfering;
}
