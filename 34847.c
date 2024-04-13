DetachOutputGPU(ScreenPtr slave)
{
    assert(slave->isGPU);
    assert(slave->is_output_slave);
    slave->current_master->output_slaves--;
    slave->is_output_slave = FALSE;
}
