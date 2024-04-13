SWTPM_CheckHMAC(tlv_data *hmac, tlv_data *encrypted_data,
                const TPM_SYMMETRIC_KEY_DATA *tpm_symmetric_key_token,
                const unsigned char *ivec, uint32_t ivec_length)
{
    const unsigned char *data;
    uint32_t data_length;
    unsigned int md_len;
    unsigned char md[EVP_MAX_MD_SIZE];

    md_len = EVP_MD_size(EVP_sha256());
    if (md_len > hmac->tlv.length) {
        logprintf(STDOUT_FILENO, "Insufficient bytes for CheckHMAC()\n");
        return TPM_FAIL;
    }

    data = encrypted_data->u.ptr;
    data_length = encrypted_data->tlv.length;

    if (!SWTPM_HMAC(md, &md_len,
                    tpm_symmetric_key_token->userKey,
                    tpm_symmetric_key_token->userKeyLength,
                    data, data_length, ivec, ivec_length)) {
        logprintf(STDOUT_FILENO, "HMAC() call failed.\n");
        return TPM_FAIL;
    }

    if (memcmp(hmac->u.ptr, md, md_len)) {
        logprintf(STDOUT_FILENO, "Verification of HMAC failed. "
                  "Data integrity is compromised\n");
        /* TPM_DECRYPT_ERROR indicates (to libtpms) that something
           exists but we have the wrong key. */
        return TPM_DECRYPT_ERROR;
    }

    return TPM_SUCCESS;
}