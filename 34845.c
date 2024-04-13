AttachUnboundGPU(ScreenPtr pScreen, ScreenPtr new)
{
    assert(new->isGPU);
    assert(!new->current_master);
    xorg_list_add(&new->slave_head, &pScreen->slave_list);
    new->current_master = pScreen;
}
