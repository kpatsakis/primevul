static lsi_request *get_pending_req(LSIState *s)
{
    lsi_request *p;

    QTAILQ_FOREACH(p, &s->queue, next) {
        if (p->pending) {
            return p;
        }
    }
    return NULL;
}
