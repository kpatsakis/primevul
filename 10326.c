SWTPM_NVRAM_PrependHeader(unsigned char **data, uint32_t *length,
                          uint16_t flags)
{
    unsigned char *out = NULL;
    uint32_t out_len = sizeof(blobheader) + *length;
    blobheader bh = {
        .version = BLOB_HEADER_VERSION,
        .min_version = 1,
        .hdrsize = htons(sizeof(bh)),
        .flags = htons(flags),
        .totlen = htonl(out_len),
    };
    TPM_RESULT res;

    out = malloc(out_len);
    if (!out) {
        logprintf(STDERR_FILENO,
                  "Could not allocate %u bytes.\n", out_len);
        res = TPM_FAIL;
        goto error;
    }

    memcpy(out, &bh, sizeof(bh));
    memcpy(&out[sizeof(bh)], *data, *length);

    free(*data);

    *data = out;
    *length = out_len;

    return TPM_SUCCESS;

 error:
    free(*data);
    *data = NULL;
    *length = 0;

    return res;
}