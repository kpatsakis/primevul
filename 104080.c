static void ide_cmd_done(IDEState *s)
{
    if (s->bus->dma->ops->cmd_done) {
        s->bus->dma->ops->cmd_done(s->bus->dma);
    }
}
