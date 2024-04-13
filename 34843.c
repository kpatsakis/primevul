AttachOffloadGPU(ScreenPtr pScreen, ScreenPtr new)
{
    assert(new->isGPU);
    assert(!new->is_offload_slave);
    assert(new->current_master == pScreen);
    new->is_offload_slave = TRUE;
}
