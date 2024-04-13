int MDC2_Init(MDC2_CTX *c)
{
    c->num = 0;
    c->pad_type = 1;
    memset(&(c->h[0]), 0x52, MDC2_BLOCK);
    memset(&(c->hh[0]), 0x25, MDC2_BLOCK);
    return 1;
}
