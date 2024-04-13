static int ebcdic_new(BIO *bi)
{
    EBCDIC_OUTBUFF *wbuf;

    wbuf = (EBCDIC_OUTBUFF *) OPENSSL_malloc(sizeof(EBCDIC_OUTBUFF) + 1024);
    if (!wbuf)
        return 0;
    wbuf->alloced = 1024;
    wbuf->buff[0] = '\0';

    bi->ptr = (char *)wbuf;
    bi->init = 1;
    bi->flags = 0;
    return (1);
}
