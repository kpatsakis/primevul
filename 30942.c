DetachFromMaster(DeviceIntPtr dev)
{
    if (IsFloating(dev))
        return;

    dev->saved_master_id = GetMaster(dev, MASTER_ATTACHED)->id;

    AttachDevice(NULL, dev, NULL);
}
