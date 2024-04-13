DeliverEventToWindowMask(DeviceIntPtr dev, WindowPtr win, xEvent *events,
                         int count, Mask filter, GrabPtr grab,
                         ClientPtr *client_return, Mask *mask_return)
{
    InputClients *iclients;

    if (!GetClientsForDelivery(dev, win, events, filter, &iclients))
        return EVENT_SKIP;

    return DeliverEventToInputClients(dev, iclients, win, events, count, filter,
                                      grab, client_return, mask_return);

}
