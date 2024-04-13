DetachOffloadGPU(ScreenPtr slave)
{
    assert(slave->isGPU);
    xorg_list_del(&slave->offload_head);
    slave->current_master = NULL;
}
