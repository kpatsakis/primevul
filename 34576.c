DetachUnboundGPU(ScreenPtr slave)
{
    assert(slave->isGPU);
    xorg_list_del(&slave->unattached_head);
    slave->current_master = NULL;
}
