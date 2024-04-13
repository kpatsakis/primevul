CreatePointerBarrierClient(ClientPtr client,
                           xXFixesCreatePointerBarrierReq * stuff,
                           PointerBarrierClientPtr *client_out)
{
    WindowPtr pWin;
    ScreenPtr screen;
    BarrierScreenPtr cs;
    int err;
    int size;
    int i;
    struct PointerBarrierClient *ret;
    CARD16 *in_devices;
    DeviceIntPtr dev;

    size = sizeof(*ret) + sizeof(DeviceIntPtr) * stuff->num_devices;
    ret = malloc(size);

    if (!ret) {
        return BadAlloc;
    }

    xorg_list_init(&ret->per_device);

    err = dixLookupWindow(&pWin, stuff->window, client, DixReadAccess);
    if (err != Success) {
        client->errorValue = stuff->window;
        goto error;
    }

    screen = pWin->drawable.pScreen;
    cs = GetBarrierScreen(screen);

    ret->screen = screen;
    ret->window = stuff->window;
    ret->num_devices = stuff->num_devices;
    if (ret->num_devices > 0)
        ret->device_ids = (int*)&ret[1];
    else
        ret->device_ids = NULL;

    in_devices = (CARD16 *) &stuff[1];
    for (i = 0; i < stuff->num_devices; i++) {
        int device_id = in_devices[i];
        DeviceIntPtr device;

        if ((err = dixLookupDevice (&device, device_id,
                                    client, DixReadAccess))) {
            client->errorValue = device_id;
            goto error;
        }

        if (!IsMaster (device)) {
            client->errorValue = device_id;
            err = BadDevice;
            goto error;
        }

        ret->device_ids[i] = device_id;
    }

    /* Alloc one per master pointer, they're the ones that can be blocked */
    xorg_list_init(&ret->per_device);
    nt_list_for_each_entry(dev, inputInfo.devices, next) {
        struct PointerBarrierDevice *pbd;

        if (dev->type != MASTER_POINTER)
            continue;

        pbd = AllocBarrierDevice();
        if (!pbd) {
            err = BadAlloc;
            goto error;
        }
        pbd->deviceid = dev->id;

        xorg_list_add(&pbd->entry, &ret->per_device);
    }

    ret->id = stuff->barrier;
    ret->barrier.x1 = stuff->x1;
    ret->barrier.x2 = stuff->x2;
    ret->barrier.y1 = stuff->y1;
    ret->barrier.y2 = stuff->y2;
    sort_min_max(&ret->barrier.x1, &ret->barrier.x2);
    sort_min_max(&ret->barrier.y1, &ret->barrier.y2);
    ret->barrier.directions = stuff->directions & 0x0f;
    if (barrier_is_horizontal(&ret->barrier))
        ret->barrier.directions &= ~(BarrierPositiveX | BarrierNegativeX);
    if (barrier_is_vertical(&ret->barrier))
        ret->barrier.directions &= ~(BarrierPositiveY | BarrierNegativeY);
    xorg_list_add(&ret->entry, &cs->barriers);

    *client_out = ret;
    return Success;

 error:
    *client_out = NULL;
    FreePointerBarrierClient(ret);
    return err;
}
