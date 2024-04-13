SWTPM_NVRAM_KeyParamCheck(uint32_t keylen,
                          enum encryption_mode encmode)
{
    TPM_RESULT rc = 0;

    if (keylen != SWTPM_AES128_BLOCK_SIZE &&
        keylen != SWTPM_AES256_BLOCK_SIZE) {
        rc = TPM_BAD_KEY_PROPERTY;
    }
    switch (encmode) {
    case ENCRYPTION_MODE_AES_CBC:
        break;
    case ENCRYPTION_MODE_UNKNOWN:
        rc = TPM_BAD_MODE;
    }

    return rc;
}