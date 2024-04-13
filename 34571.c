AttachUnboundGPU(ScreenPtr pScreen, ScreenPtr new)
{
    assert(new->isGPU);
    assert(!new->current_master);
    xorg_list_add(&new->unattached_head, &pScreen->unattached_list);
    new->current_master = pScreen;
}
