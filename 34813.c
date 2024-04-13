static struct PointerBarrierDevice *GetBarrierDevice(struct PointerBarrierClient *c, int deviceid)
{
    struct PointerBarrierDevice *pbd = NULL;

    xorg_list_for_each_entry(pbd, &c->per_device, entry) {
        if (pbd->deviceid == deviceid)
            break;
    }

    BUG_WARN(!pbd);
    return pbd;
}
