const SSL_CIPHER *dtls1_get_cipher(unsigned int u)
{
    const SSL_CIPHER *ciph = ssl3_get_cipher(u);

    if (ciph != NULL) {
        if (ciph->algorithm_enc == SSL_RC4)
            return NULL;
    }

    return ciph;
}
