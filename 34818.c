barrier_blocks_device(struct PointerBarrierClient *client,
                      DeviceIntPtr dev)
{
    int i;
    int master_id;

    /* Clients with no devices are treated as
     * if they specified XIAllDevices. */
    if (client->num_devices == 0)
        return TRUE;

    master_id = GetMaster(dev, POINTER_OR_FLOAT)->id;

    for (i = 0; i < client->num_devices; i++) {
        int device_id = client->device_ids[i];
        if (device_id == XIAllDevices ||
            device_id == XIAllMasterDevices ||
            device_id == master_id)
            return TRUE;
    }

    return FALSE;
}
