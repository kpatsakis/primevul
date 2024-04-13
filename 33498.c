static BIO *cms_get_text_bio(BIO *out, unsigned int flags)
{
    BIO *rbio;
    if (out == NULL)
        rbio = BIO_new(BIO_s_null());
    else if (flags & CMS_TEXT) {
        rbio = BIO_new(BIO_s_mem());
        BIO_set_mem_eof_return(rbio, 0);
    } else
        rbio = out;
    return rbio;
}
