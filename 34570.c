AttachOutputGPU(ScreenPtr pScreen, ScreenPtr new)
{
    assert(new->isGPU);
    xorg_list_add(&new->output_head, &pScreen->output_slave_list);
    new->current_master = pScreen;
}
