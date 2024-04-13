void ide_dma_error(IDEState *s)
{
    dma_buf_commit(s, 0);
    ide_abort_command(s);
    ide_set_inactive(s, false);
    ide_set_irq(s->bus);
}
