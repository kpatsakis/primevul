AttachOutputGPU(ScreenPtr pScreen, ScreenPtr new)
{
    assert(new->isGPU);
    assert(!new->is_output_slave);
    assert(new->current_master == pScreen);
    new->is_output_slave = TRUE;
    new->current_master->output_slaves++;
}
