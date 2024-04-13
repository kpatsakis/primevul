TPM_RESULT SWTPM_NVRAM_Set_MigrationKey(const unsigned char *key,
                                        uint32_t keylen,
                                        enum encryption_mode encmode)
{
    TPM_RESULT rc;

    rc = SWTPM_NVRAM_KeyParamCheck(keylen, encmode);

    if (rc == 0) {
        memcpy(migrationkey.symkey.userKey, key, keylen);
        migrationkey.symkey.userKeyLength = keylen;
        migrationkey.data_encmode = encmode;
    }

    return rc;
}