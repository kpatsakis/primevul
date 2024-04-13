void ide_set_inactive(IDEState *s, bool more)
{
    s->bus->dma->aiocb = NULL;
    if (s->bus->dma->ops->set_inactive) {
        s->bus->dma->ops->set_inactive(s->bus->dma, more);
    }
    ide_cmd_done(s);
}
