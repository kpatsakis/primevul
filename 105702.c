static void *rndis_queue_response(USBNetState *s, unsigned int length)
{
    struct rndis_response *r =
            g_malloc0(sizeof(struct rndis_response) + length);

    if (QTAILQ_EMPTY(&s->rndis_resp)) {
        usb_wakeup(s->intr, 0);
    }

    QTAILQ_INSERT_TAIL(&s->rndis_resp, r, entries);
    r->length = length;

    return &r->buf[0];
}
