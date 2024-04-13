DetachOutputGPU(ScreenPtr slave)
{
    assert(slave->isGPU);
    xorg_list_del(&slave->output_head);
    slave->current_master = NULL;
}
