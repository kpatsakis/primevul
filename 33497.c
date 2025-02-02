static int cms_copy_content(BIO *out, BIO *in, unsigned int flags)
{
    unsigned char buf[4096];
    int r = 0, i;
    BIO *tmpout;

    tmpout = cms_get_text_bio(out, flags);

    if (tmpout == NULL) {
        CMSerr(CMS_F_CMS_COPY_CONTENT, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    /* Read all content through chain to process digest, decrypt etc */
    for (;;) {
        i = BIO_read(in, buf, sizeof(buf));
        if (i <= 0) {
            if (BIO_method_type(in) == BIO_TYPE_CIPHER) {
                if (!BIO_get_cipher_status(in))
                    goto err;
            }
            if (i < 0)
                goto err;
            break;
        }

        if (tmpout && (BIO_write(tmpout, buf, i) != i))
            goto err;
    }

    if (flags & CMS_TEXT) {
        if (!SMIME_text(tmpout, out)) {
            CMSerr(CMS_F_CMS_COPY_CONTENT, CMS_R_SMIME_TEXT_ERROR);
            goto err;
        }
    }

    r = 1;

 err:
    if (tmpout != out)
        BIO_free(tmpout);
    return r;

}
