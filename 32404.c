static int tls1_check_duplicate_extensions(const PACKET *packet)
{
    PACKET extensions = *packet;
    size_t num_extensions = 0, i = 0;
    unsigned int *extension_types = NULL;
    int ret = 0;

    /* First pass: count the extensions. */
    while (PACKET_remaining(&extensions) > 0) {
        unsigned int type;
        PACKET extension;
        if (!PACKET_get_net_2(&extensions, &type) ||
            !PACKET_get_length_prefixed_2(&extensions, &extension)) {
            goto done;
        }
        num_extensions++;
    }

    if (num_extensions <= 1)
        return 1;

    extension_types = OPENSSL_malloc(sizeof(unsigned int) * num_extensions);
    if (extension_types == NULL) {
        SSLerr(SSL_F_TLS1_CHECK_DUPLICATE_EXTENSIONS, ERR_R_MALLOC_FAILURE);
        goto done;
    }

    /* Second pass: gather the extension types. */
    extensions = *packet;
    for (i = 0; i < num_extensions; i++) {
        PACKET extension;
        if (!PACKET_get_net_2(&extensions, &extension_types[i]) ||
            !PACKET_get_length_prefixed_2(&extensions, &extension)) {
            /* This should not happen. */
            SSLerr(SSL_F_TLS1_CHECK_DUPLICATE_EXTENSIONS, ERR_R_INTERNAL_ERROR);
            goto done;
        }
    }

    if (PACKET_remaining(&extensions) != 0) {
        SSLerr(SSL_F_TLS1_CHECK_DUPLICATE_EXTENSIONS, ERR_R_INTERNAL_ERROR);
        goto done;
    }
    /* Sort the extensions and make sure there are no duplicates. */
    qsort(extension_types, num_extensions, sizeof(unsigned int), compare_uint);
    for (i = 1; i < num_extensions; i++) {
        if (extension_types[i - 1] == extension_types[i])
            goto done;
    }
    ret = 1;
 done:
    OPENSSL_free(extension_types);
    return ret;
}
