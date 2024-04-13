IsPointerDevice(DeviceIntPtr dev)
{
    return (dev->type == MASTER_POINTER) ||
        (dev->valuator && dev->button) || (dev->valuator && !dev->key);
}
