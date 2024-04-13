static TPM_RESULT SWTPM_RollAndSetGlobalIvec(tlv_data *td,
                                             uint16_t tag_ivec,
                                             uint32_t ivec_length)
{
    unsigned char data[16]; /* do not initialize */
    unsigned char hashbuf[SHA256_DIGEST_LENGTH];
    void *p;

    if (g_ivec_length < ivec_length) {
        p = realloc(g_ivec, ivec_length);
        if (!p) {
            *td = TLV_DATA_CONST(tag_ivec, 0, NULL);

            logprintf(STDOUT_FILENO,
                      "Could not allocate %u bytes.\n", ivec_length);
            return TPM_FAIL;
        }
        g_ivec = p;
        g_ivec_length = ivec_length;
    }

    if (RAND_bytes(g_ivec, g_ivec_length) != 1) {
        /* random data from stack to the rescue */
        SHA256(g_ivec, g_ivec_length, hashbuf);
        SHA256(data, sizeof(data), hashbuf);
        memcpy(g_ivec, hashbuf,
               g_ivec_length < sizeof(hashbuf)
                   ? g_ivec_length
                   : sizeof(hashbuf));
    }

    *td = TLV_DATA_CONST(tag_ivec, g_ivec_length, g_ivec);

    return 0;
}