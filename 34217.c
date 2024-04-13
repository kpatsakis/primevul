static int ebcdic_free(BIO *a)
{
    if (a == NULL)
        return (0);
    if (a->ptr != NULL)
        OPENSSL_free(a->ptr);
    a->ptr = NULL;
    a->init = 0;
    a->flags = 0;
    return (1);
}
