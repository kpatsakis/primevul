IsFloating(DeviceIntPtr dev)
{
    return !IsMaster(dev) && GetMaster(dev, MASTER_KEYBOARD) == NULL;
}
