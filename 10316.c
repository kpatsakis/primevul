SWTPM_CheckHash(const unsigned char *in, uint32_t in_length,
                unsigned char **out, uint32_t *out_length)
{
    TPM_RESULT rc = 0;
    unsigned char *dest = NULL;
    unsigned char hashbuf[SHA256_DIGEST_LENGTH];
    const unsigned char *data = &in[sizeof(hashbuf)];
    uint32_t data_length = in_length - sizeof(hashbuf);

    /* hash the data */
    SHA256(data, data_length, hashbuf);

    if (memcmp(in, hashbuf, sizeof(hashbuf))) {
        logprintf(STDOUT_FILENO, "Verification of hash failed. "
                  "Data integrity is compromised\n");
        rc = TPM_FAIL;
    }

    if (rc == 0) {
        dest = malloc(data_length);
        if (dest) {
            *out = dest;
            *out_length = data_length;
            memcpy(dest, data, data_length);
        } else {
            logprintf(STDOUT_FILENO,
                      "Could not allocated %u bytes.\n", data_length);
            rc = TPM_FAIL;
        }
    }

    return rc;
}