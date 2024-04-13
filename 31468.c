int i2d_ECPrivateKey(EC_KEY *a, unsigned char **out)
{
    int ret = 0, ok = 0;
    unsigned char *buffer = NULL;
    size_t buf_len = 0, tmp_len;
    EC_PRIVATEKEY *priv_key = NULL;

    if (a == NULL || a->group == NULL || a->priv_key == NULL) {
        ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_PASSED_NULL_PARAMETER);
        goto err;
    }

    if ((priv_key = EC_PRIVATEKEY_new()) == NULL) {
        ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    priv_key->version = a->version;

    buf_len = (size_t)BN_num_bytes(a->priv_key);
    buffer = OPENSSL_malloc(buf_len);
    if (buffer == NULL) {
        ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    if (!BN_bn2bin(a->priv_key, buffer)) {
        ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_BN_LIB);
        goto err;
    }

    if (!M_ASN1_OCTET_STRING_set(priv_key->privateKey, buffer, buf_len)) {
        ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_ASN1_LIB);
        goto err;
    }

    if (!(a->enc_flag & EC_PKEY_NO_PARAMETERS)) {
        if ((priv_key->parameters =
             ec_asn1_group2pkparameters(a->group,
                                        priv_key->parameters)) == NULL) {
            ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_EC_LIB);
            goto err;
        }
    }

    if (!(a->enc_flag & EC_PKEY_NO_PUBKEY)) {
        priv_key->publicKey = M_ASN1_BIT_STRING_new();
        if (priv_key->publicKey == NULL) {
            ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_MALLOC_FAILURE);
            goto err;
        }

        tmp_len = EC_POINT_point2oct(a->group, a->pub_key,
                                     a->conv_form, NULL, 0, NULL);

        if (tmp_len > buf_len) {
            unsigned char *tmp_buffer = OPENSSL_realloc(buffer, tmp_len);
            if (!tmp_buffer) {
                ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_MALLOC_FAILURE);
                goto err;
            }
            buffer = tmp_buffer;
            buf_len = tmp_len;
        }

        if (!EC_POINT_point2oct(a->group, a->pub_key,
                                a->conv_form, buffer, buf_len, NULL)) {
            ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_EC_LIB);
            goto err;
        }

        priv_key->publicKey->flags &= ~(ASN1_STRING_FLAG_BITS_LEFT | 0x07);
        priv_key->publicKey->flags |= ASN1_STRING_FLAG_BITS_LEFT;
        if (!M_ASN1_BIT_STRING_set(priv_key->publicKey, buffer, buf_len)) {
            ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_ASN1_LIB);
            goto err;
        }
    }

    if ((ret = i2d_EC_PRIVATEKEY(priv_key, out)) == 0) {
        ECerr(EC_F_I2D_ECPRIVATEKEY, ERR_R_EC_LIB);
        goto err;
    }
    ok = 1;
 err:
    if (buffer)
        OPENSSL_free(buffer);
    if (priv_key)
        EC_PRIVATEKEY_free(priv_key);
    return (ok ? ret : 0);
}
