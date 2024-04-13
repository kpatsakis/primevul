void ide_start_dma(IDEState *s, BlockCompletionFunc *cb)
{
    if (s->bus->dma->ops->start_dma) {
        s->bus->dma->ops->start_dma(s->bus->dma, s, cb);
    }
}
