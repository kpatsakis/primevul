ReattachToOldMaster(DeviceIntPtr dev)
{
    DeviceIntPtr master = NULL;

    if (IsMaster(dev))
        return;

    dixLookupDevice(&master, dev->saved_master_id, serverClient, DixUseAccess);

    if (master) {
        AttachDevice(serverClient, dev, master);
        dev->saved_master_id = 0;
    }
}
