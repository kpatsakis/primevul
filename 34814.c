void XIBarrierNewMasterDevice(ClientPtr client, int deviceid)
{
    FindClientResourcesByType(client, PointerBarrierType, add_master_func, &deviceid);
}
