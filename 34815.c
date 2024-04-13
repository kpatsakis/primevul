void XIBarrierRemoveMasterDevice(ClientPtr client, int deviceid)
{
    FindClientResourcesByType(client, PointerBarrierType, remove_master_func, &deviceid);
}
