SWTPM_NVRAM_StoreData_Intern(const unsigned char *data,
                             uint32_t length,
                             uint32_t tpm_number,
                             const char *name,
                             TPM_BOOL encrypt         /* encrypt if key is set */)
{
    TPM_RESULT    rc = 0;
    unsigned char *filedata = NULL;
    uint32_t      filedata_length = 0;
    tlv_data      td[3];
    size_t        td_len = 0;
    uint16_t      flags = 0;
    const char    *backend_uri = NULL;

    TPM_DEBUG(" SWTPM_NVRAM_StoreData: To name %s\n", name);

    if (rc == 0) {
        if (encrypt && SWTPM_NVRAM_Has_FileKey()) {
            td_len = 3;
            rc = SWTPM_NVRAM_EncryptData(&filekey, &td[0], &td_len,
                                         TAG_ENCRYPTED_DATA, data, length,
                                         TAG_IVEC_ENCRYPTED_DATA);
            if (rc) {
                logprintf(STDERR_FILENO,
                          "SWTPM_NVRAM_EncryptData failed: 0x%02x\n", rc);
            } else {
                TPM_DEBUG("  SWTPM_NVRAM_StoreData: Encrypted %u bytes before "
                          "write, will write %u bytes\n", length,
                          td[0].tlv.length);
            }
            flags |= BLOB_FLAG_ENCRYPTED;
            if (SWTPM_NVRAM_FileKey_Size() == SWTPM_AES256_BLOCK_SIZE)
                flags |= BLOB_FLAG_ENCRYPTED_256BIT_KEY;
        } else {
            td_len = 1;
            td[0] = TLV_DATA_CONST(TAG_DATA, length, data);
        }
    }

    if (rc == 0)
        rc = tlv_data_append(&filedata, &filedata_length, td, td_len);

    if (rc == 0)
        rc = SWTPM_NVRAM_PrependHeader(&filedata, &filedata_length, flags);

    if (rc == 0) {
        backend_uri = tpmstate_get_backend_uri();
        rc = g_nvram_backend_ops->store(filedata, filedata_length, tpm_number, name,
                                        backend_uri);
    }

    tlv_data_free(td, td_len);
    free(filedata);

    TPM_DEBUG(" SWTPM_NVRAM_StoreData: rc=%d\n", rc);

    return rc;
}