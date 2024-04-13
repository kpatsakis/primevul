static void lsi_queue_command(LSIState *s)
{
    lsi_request *p = s->current;

    trace_lsi_queue_command(p->tag);
    assert(s->current != NULL);
    assert(s->current->dma_len == 0);
    QTAILQ_INSERT_TAIL(&s->queue, s->current, next);
    s->current = NULL;

    p->pending = 0;
    p->out = (s->sstat1 & PHASE_MASK) == PHASE_DO;
}
