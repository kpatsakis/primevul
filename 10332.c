TPM_RESULT SWTPM_NVRAM_GetStateBlob(unsigned char **data,
                                    uint32_t *length,
                                    uint32_t tpm_number,
                                    const char *name,
                                    TPM_BOOL decrypt,
                                    TPM_BOOL *is_encrypted)
{
    TPM_RESULT res;
    uint16_t flags = 0;
    tlv_data td[3];
    size_t td_len;
    unsigned char *plain = NULL, *buffer = NULL;
    uint32_t plain_len, buffer_len = 0;

    *data = NULL;
    *length = 0;

    res = SWTPM_NVRAM_LoadData(&plain, &plain_len, tpm_number, name);
    if (res)
        return res;

    /* @plain contains unencrypted data without tlv headers */

    /* if the user doesn't want decryption and there's a file key, we need to
       encrypt the data */
    if (!decrypt && SWTPM_NVRAM_Has_FileKey()) {
        td_len = 3;
        res = SWTPM_NVRAM_EncryptData(&filekey, &td[0], &td_len,
                                      TAG_ENCRYPTED_DATA, plain, plain_len,
                                      TAG_IVEC_ENCRYPTED_DATA);
        if (res)
            goto err_exit;

        *is_encrypted = TRUE;
        if (SWTPM_NVRAM_FileKey_Size() == SWTPM_AES256_BLOCK_SIZE)
            flags |= BLOB_FLAG_ENCRYPTED_256BIT_KEY;
    } else {
        *is_encrypted = FALSE;
        td[0] = TLV_DATA(TAG_DATA, plain_len, plain);
        plain = NULL;
        td_len = 1;
    }

    res = tlv_data_append(&buffer, &buffer_len, td, td_len);
    if (res)
        goto err_exit;

    tlv_data_free(td, td_len);

    /* @buffer contains tlv data */

    if (SWTPM_NVRAM_Has_MigrationKey()) {
        /* we have to encrypt it now with the migration key */
        flags |= BLOB_FLAG_MIGRATION_ENCRYPTED;
        if (SWTPM_NVRAM_MigrationKey_Size() == SWTPM_AES256_BLOCK_SIZE)
             flags |= BLOB_FLAG_MIGRATION_256BIT_KEY;

        td_len = 3;
        res = SWTPM_NVRAM_EncryptData(&migrationkey, &td[0], &td_len,
                                      TAG_ENCRYPTED_MIGRATION_DATA,
                                      buffer, buffer_len,
                                      TAG_IVEC_ENCRYPTED_MIGRATION_DATA);
        if (res)
            goto err_exit;
    } else {
        td[0] = TLV_DATA(TAG_MIGRATION_DATA, buffer_len, buffer);
        buffer = NULL;
        td_len = 1;
    }
    flags |= BLOB_FLAG_MIGRATION_DATA;

    res = tlv_data_append(data, length, td, td_len);
    if (res)
        goto err_exit;

    /* put the header in clear text */
    if (*is_encrypted)
        flags |= BLOB_FLAG_ENCRYPTED;

    res = SWTPM_NVRAM_PrependHeader(data, length, flags);

err_exit:
    tlv_data_free(td, td_len);
    free(buffer);
    free(plain);

    return res;
}