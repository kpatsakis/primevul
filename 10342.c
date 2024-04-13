SWTPM_NVRAM_DecryptData(const encryptionkey *key,
                        unsigned char **decrypt_data, uint32_t *decrypt_length,
                        const unsigned char *data, uint32_t length,
                        uint16_t tag_encrypted_data,
                        uint8_t hdrversion,
                        uint16_t tag_ivec, uint16_t hdrflags,
                        uint16_t flag_256bitkey)
{
    TPM_RESULT rc = 0;
    unsigned char *tmp_data = NULL;
    uint32_t tmp_length = 0;
    tlv_data td[2];
    const unsigned char *ivec = NULL;
    uint32_t ivec_length = 0;
    size_t keylen;

    if (key->symkey.userKeyLength > 0) {
        switch (key->data_encmode) {
        case ENCRYPTION_MODE_UNKNOWN:
            rc = TPM_BAD_MODE;
            break;
        case ENCRYPTION_MODE_AES_CBC:
            switch (hdrversion) {
            case 1:
                rc = SWTPM_SymmetricKeyData_Decrypt(&tmp_data,
                                                    &tmp_length,
                                                    data, length,
                                                    &key->symkey,
                                                    NULL, 0);
                if (rc == 0) {
                    rc = SWTPM_CheckHash(tmp_data, tmp_length,
                                         decrypt_data, decrypt_length);
                }
            break;
            case 2:
                keylen = (hdrflags & flag_256bitkey)
                          ? SWTPM_AES256_BLOCK_SIZE : SWTPM_AES128_BLOCK_SIZE;
                if (keylen != key->symkey.userKeyLength) {
                    logprintf(STDERR_FILENO,
                              "Wrong decryption key. Need %zu bit key.\n",
                              keylen * 8);
                    rc = TPM_BAD_KEY_PROPERTY;
                    break;
                }

                if (!tlv_data_find_tag(data, length, TAG_HMAC, &td[0]) ||
                    !tlv_data_find_tag(data, length, tag_encrypted_data,
                                       &td[1])) {
                    logprintf(STDERR_FILENO,
                              "Could not find HMAC or encrypted data (tag %u) "
                              "in byte stream.\n", tag_encrypted_data);
                    rc = TPM_FAIL;
                    break;
                }
                /* get the IV, if there is one */
                SWTPM_GetIvec(data, length, &ivec, &ivec_length, tag_ivec);

                rc = SWTPM_CheckHMAC(&td[0], &td[1], &key->symkey,
                                     ivec, ivec_length);
                if (rc == 0) {
                    rc = SWTPM_SymmetricKeyData_Decrypt(decrypt_data,
                                                        decrypt_length,
                                                        td[1].u.const_ptr,
                                                        td[1].tlv.length,
                                                        &key->symkey,
                                                        ivec, ivec_length);
                }
            break;
            default:
                rc = TPM_FAIL;
            }
            free(tmp_data);
        }
    }

    return rc;
}