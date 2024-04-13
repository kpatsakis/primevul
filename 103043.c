static lsi_request *lsi_find_by_tag(LSIState *s, uint32_t tag)
{
    lsi_request *p;

    QTAILQ_FOREACH(p, &s->queue, next) {
        if (p->tag == tag) {
            return p;
        }
    }

    return NULL;
}
