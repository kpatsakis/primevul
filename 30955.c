GetClientsForDelivery(DeviceIntPtr dev, WindowPtr win,
                      xEvent *events, Mask filter, InputClients ** iclients)
{
    int rc = 0;

    if (core_get_type(events) != 0)
        *iclients = (InputClients *) wOtherClients(win);
    else if (xi2_get_type(events) != 0) {
        OtherInputMasks *inputMasks = wOtherInputMasks(win);

        /* Has any client selected for the event? */
        if (!WindowXI2MaskIsset(dev, win, events))
            goto out;
        *iclients = inputMasks->inputClients;
    }
    else {
        OtherInputMasks *inputMasks = wOtherInputMasks(win);

        /* Has any client selected for the event? */
        if (!inputMasks || !(inputMasks->inputEvents[dev->id] & filter))
            goto out;

        *iclients = inputMasks->inputClients;
    }

    rc = 1;
 out:
    return rc;
}
