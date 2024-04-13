DeliverRawEvent(RawDeviceEvent *ev, DeviceIntPtr device)
{
    GrabPtr grab = device->deviceGrab.grab;
    xEvent *xi;
    int i, rc;
    int filter;

    rc = EventToXI2((InternalEvent *) ev, (xEvent **) &xi);
    if (rc != Success) {
        ErrorF("[Xi] %s: XI2 conversion failed in %s (%d)\n",
               __func__, device->name, rc);
        return;
    }

    if (grab)
        DeliverGrabbedEvent((InternalEvent *) ev, device, FALSE);

    filter = GetEventFilter(device, xi);

    for (i = 0; i < screenInfo.numScreens; i++) {
        WindowPtr root;
        InputClients *inputclients;

        root = screenInfo.screens[i]->root;
        if (!GetClientsForDelivery(device, root, xi, filter, &inputclients))
            continue;

        for (; inputclients; inputclients = inputclients->next) {
            ClientPtr c;        /* unused */
            Mask m;             /* unused */
            InputClients ic = *inputclients;

            /* Because we run through the list manually, copy the actual
             * list, shorten the copy to only have one client and then pass
             * that down to DeliverEventToInputClients. This way we avoid
             * double events on XI 2.1 clients that have a grab on the
             * device.
             */
            ic.next = NULL;

            if (!FilterRawEvents(rClient(&ic), grab, root))
                DeliverEventToInputClients(device, &ic, root, xi, 1,
                                           filter, NULL, &c, &m);
        }
    }

    free(xi);
}
