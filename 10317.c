TPM_RESULT SWTPM_NVRAM_Set_FileKey(const unsigned char *key, uint32_t keylen,
                                   enum encryption_mode encmode)
{
    TPM_RESULT rc;

    rc = SWTPM_NVRAM_KeyParamCheck(keylen, encmode);

    if (rc == 0) {
        memcpy(filekey.symkey.userKey, key, keylen);
        filekey.symkey.userKeyLength = keylen;
        filekey.data_encmode = encmode;
    }

    return rc;
}