DetachOffloadGPU(ScreenPtr slave)
{
    assert(slave->isGPU);
    assert(slave->is_offload_slave);
    slave->is_offload_slave = FALSE;
}
