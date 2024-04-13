static void dma_buf_commit(IDEState *s, uint32_t tx_bytes)
{
    if (s->bus->dma->ops->commit_buf) {
        s->bus->dma->ops->commit_buf(s->bus->dma, tx_bytes);
    }
    qemu_sglist_destroy(&s->sg);
}
