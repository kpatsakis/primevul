SWTPM_NVRAM_GetDecryptedData(const encryptionkey *key,
                             unsigned char **decrypt_data,
                             uint32_t *decrypt_length,
                             const unsigned char *data,
                             uint32_t length,
                             uint16_t tag_encrypted_data,
                             uint16_t tag_data,
                             uint8_t hdrversion,
                             uint16_t tag_ivec,
                             uint16_t hdrflags,
                             uint16_t flag_256bitkey)
{
    if (key && key->symkey.userKeyLength > 0) {
        /* we assume the data are encrypted when there's a key given */
        return SWTPM_NVRAM_DecryptData(key, decrypt_data, decrypt_length,
                                       data, length, tag_encrypted_data,
                                       hdrversion, tag_ivec, hdrflags,
                                       flag_256bitkey);
    }
    return SWTPM_NVRAM_GetPlainData(decrypt_data, decrypt_length,
                                    data, length, tag_data, hdrversion);
}