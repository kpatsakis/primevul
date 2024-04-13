IsMaster(DeviceIntPtr dev)
{
    return dev->type == MASTER_POINTER || dev->type == MASTER_KEYBOARD;
}
