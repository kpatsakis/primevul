inf_gtk_certificate_manager_compare_fingerprint(gnutls_x509_crt_t cert1,
                                                gnutls_x509_crt_t cert2,
                                                GError** error)
{
  static const unsigned int SHA256_DIGEST_SIZE = 32;

  size_t size;
  guchar cert1_fingerprint[SHA256_DIGEST_SIZE];
  guchar cert2_fingerprint[SHA256_DIGEST_SIZE];

  int ret;
  int cmp;

  size = SHA256_DIGEST_SIZE;

  ret = gnutls_x509_crt_get_fingerprint(
    cert1,
    GNUTLS_DIG_SHA256,
    cert1_fingerprint,
    &size
  );

  if(ret == GNUTLS_E_SUCCESS)
  {
    g_assert(size == SHA256_DIGEST_SIZE);

    ret = gnutls_x509_crt_get_fingerprint(
      cert2,
      GNUTLS_DIG_SHA256,
      cert2_fingerprint,
      &size
    );
  }

  if(ret != GNUTLS_E_SUCCESS)
  {
    inf_gnutls_set_error(error, ret);
    return FALSE;
  }

  cmp = memcmp(cert1_fingerprint, cert2_fingerprint, SHA256_DIGEST_SIZE);
  if(cmp != 0) return FALSE;

  return TRUE;
}
