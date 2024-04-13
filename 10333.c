SWTPM_NVRAM_EncryptData(const encryptionkey *key,
                        struct tlv_data *td, /* must provide 2 array members */
                        size_t *td_len,
                        uint16_t tag_encrypted_data,
                        const unsigned char *data, uint32_t length,
                        uint16_t tag_ivec)
{
    TPM_RESULT rc = 0;
    TPM_RESULT irc;
    unsigned char *tmp_data = NULL;
    uint32_t tmp_length = 0;

    *td_len = 0;

    if (key->symkey.userKeyLength > 0) {
        switch (key->data_encmode) {
        case ENCRYPTION_MODE_UNKNOWN:
            rc = TPM_BAD_MODE;
            break;
        case ENCRYPTION_MODE_AES_CBC:
            irc = SWTPM_RollAndSetGlobalIvec(&td[2], tag_ivec,
                                             key->symkey.userKeyLength);
            rc = SWTPM_SymmetricKeyData_Encrypt(&tmp_data, &tmp_length,
                                                data, length, &key->symkey,
                                                td[2].u.const_ptr,
                                                td[2].tlv.length);
            if (rc)
                 break;

            rc = SWTPM_CalcHMAC(tmp_data, tmp_length, &td[1], &key->symkey,
                                td[2].u.const_ptr, td[2].tlv.length);
            if (rc == 0) {
                td[0] = TLV_DATA(tag_encrypted_data, tmp_length, tmp_data);
                /* in case we couldn't get an IV */
                *td_len = (irc == 0) ? 3 : 2;
                tmp_data = NULL;
            }
            break;
        }
    }

    if (rc)
        tlv_data_free(td, *td_len);

    free(tmp_data);

    return rc;
}