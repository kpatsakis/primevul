SWTPM_NVRAM_GetPlainData(unsigned char **plain, uint32_t *plain_length,
                         const unsigned char *data, uint32_t length,
                         uint16_t tag_data,
                         uint8_t hdrversion)
{
    TPM_RESULT rc = 0;
    tlv_data td[1];

    switch (hdrversion) {
    case 1:
        *plain = malloc(length);
        if (*plain) {
            memcpy(*plain, data, length);
            *plain_length = length;
        } else {
            logprintf(STDERR_FILENO,
                      "Could not allocate %u bytes.\n", length);
            rc = TPM_FAIL;
        }
    break;

    case 2:
        if (!tlv_data_find_tag(data, length, tag_data, &td[0])) {
            logprintf(STDERR_FILENO,
                      "Could not find plain data in byte stream.\n");
            rc = TPM_FAIL;
            break;
        }
        *plain = malloc(td->tlv.length);
        if (*plain) {
            memcpy(*plain, td->u.const_ptr, td->tlv.length);
            *plain_length = td->tlv.length;
        } else {
            logprintf(STDERR_FILENO,
                      "Could not allocate %u bytes.\n", td->tlv.length);
            rc = TPM_FAIL;
        }
    break;
    }

    return rc;
}