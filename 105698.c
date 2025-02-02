static int rndis_get_response(USBNetState *s, uint8_t *buf)
{
    int ret = 0;
    struct rndis_response *r = s->rndis_resp.tqh_first;

    if (!r)
        return ret;

    QTAILQ_REMOVE(&s->rndis_resp, r, entries);
    ret = r->length;
    memcpy(buf, r->buf, r->length);
    g_free(r);

    return ret;
}
