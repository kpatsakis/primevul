_tls_set_certificate (SSL_CTX * ctx, const char *cn)
{
#ifdef HAVE_WINCRYPT_H
  PCCERT_CONTEXT pCertCtx;
  X509 *cert = NULL;
  HCERTSTORE hStore = CertOpenSystemStore (0, L"CA");

  for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
    char peer_CN[65];

    cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
    if (cert == NULL) {
      continue;
    }

    memset (peer_CN, 0, sizeof (peer_CN));
    X509_NAME_get_text_by_NID (X509_get_subject_name (cert), NID_commonName, peer_CN, sizeof (peer_CN));
    if (osip_strcasecmp (cn, peer_CN) == 0) {
      break;
    }
    X509_free (cert);
    cert = NULL;
  }

  CertCloseStore (hStore, 0);

  if (cert == NULL) {
    hStore = CertOpenSystemStore (0, L"ROOT");
    for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
      char peer_CN[65];

      cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
      if (cert == NULL) {
        continue;
      }

      memset (peer_CN, 0, sizeof (peer_CN));
      X509_NAME_get_text_by_NID (X509_get_subject_name (cert), NID_commonName, peer_CN, sizeof (peer_CN));
      if (osip_strcasecmp (cn, peer_CN) == 0) {
        break;
      }
      X509_free (cert);
      cert = NULL;
    }

    CertCloseStore (hStore, 0);
  }

  if (cert == NULL) {
    hStore = CertOpenSystemStore (0, L"MY");

    for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
      char peer_CN[65];

      cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
      if (cert == NULL) {
        continue;
      }

      memset (peer_CN, 0, sizeof (peer_CN));
      X509_NAME_get_text_by_NID (X509_get_subject_name (cert), NID_commonName, peer_CN, sizeof (peer_CN));
      if (osip_strcasecmp (cn, peer_CN) == 0) {
        break;
      }
      X509_free (cert);
      cert = NULL;
    }

    CertCloseStore (hStore, 0);
  }

  if (cert == NULL) {
    hStore = CertOpenSystemStore (0, L"Trustedpublisher");

    for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
      char peer_CN[65];

      cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
      if (cert == NULL) {
        continue;
      }

      memset (peer_CN, 0, sizeof (peer_CN));
      X509_NAME_get_text_by_NID (X509_get_subject_name (cert), NID_commonName, peer_CN, sizeof (peer_CN));
      if (osip_strcasecmp (cn, peer_CN) == 0) {
        break;
      }
      X509_free (cert);
      cert = NULL;
    }

    CertCloseStore (hStore, 0);
  }

  if (cert == NULL)
    return NULL;

  {
    RSA *rsa = NULL, *pub_rsa;
    struct rsa_ctx *priv;
    RSA_METHOD *rsa_meth;

    priv = osip_malloc (sizeof (*priv));
    rsa_meth = osip_malloc (sizeof (*rsa_meth));
    if (priv == NULL || rsa_meth == NULL) {
      CertFreeCertificateContext (pCertCtx);
      osip_free (priv);
      osip_free (rsa_meth);
      X509_free (cert);
      return NULL;
    }
    memset (priv, 0, sizeof (*priv));
    memset (rsa_meth, 0, sizeof (*rsa_meth));

    priv->cert = pCertCtx;

    if (CryptAcquireCertificatePrivateKey (pCertCtx, CRYPT_ACQUIRE_COMPARE_KEY_FLAG, NULL, &priv->crypt_prov, &priv->key_spec, &priv->free_crypt_prov) == 0) {
      CertFreeCertificateContext (priv->cert);
      osip_free (priv);
      osip_free (rsa_meth);
      X509_free (cert);
      return NULL;
    }

    if (!CryptGetUserKey (priv->crypt_prov, priv->key_spec, &priv->hCryptKey)) {
      CertFreeCertificateContext (priv->cert);
      if (priv->crypt_prov && priv->free_crypt_prov)
        CryptReleaseContext (priv->crypt_prov, 0);
      osip_free (priv);
      osip_free (rsa_meth);
      X509_free (cert);
      return NULL;
    }

    rsa_meth->name = "Microsoft CryptoAPI RSA Method";
    rsa_meth->rsa_pub_enc = rsa_pub_enc;
    rsa_meth->rsa_pub_dec = rsa_pub_dec;
    rsa_meth->rsa_priv_enc = rsa_priv_enc;
    rsa_meth->rsa_priv_dec = rsa_priv_dec;
    rsa_meth->finish = rsa_finish;
    rsa_meth->flags = RSA_METHOD_FLAG_NO_CHECK;
    rsa_meth->app_data = (char *) priv;

    rsa = RSA_new ();
    if (rsa == NULL) {
      CertFreeCertificateContext (priv->cert);
      if (priv->crypt_prov && priv->free_crypt_prov)
        CryptReleaseContext (priv->crypt_prov, 0);
      osip_free (priv);
      osip_free (rsa_meth);
      X509_free (cert);
      RSA_free (rsa);
      return NULL;
    }

    if (!SSL_CTX_use_certificate (ctx, cert)) {
      CertFreeCertificateContext (priv->cert);
      if (priv->crypt_prov && priv->free_crypt_prov)
        CryptReleaseContext (priv->crypt_prov, 0);
      osip_free (priv);
      osip_free (rsa_meth);
      X509_free (cert);
      return NULL;
    }

    pub_rsa = cert->cert_info->key->pkey->pkey.rsa;

    rsa->n = BN_dup (pub_rsa->n);
    rsa->e = BN_dup (pub_rsa->e);
    if (!RSA_set_method (rsa, rsa_meth)) {
      CertFreeCertificateContext (priv->cert);
      if (priv->crypt_prov && priv->free_crypt_prov)
        CryptReleaseContext (priv->crypt_prov, 0);
      osip_free (priv);
      osip_free (rsa_meth);
      RSA_free (rsa);
      X509_free (cert);
      SSL_CTX_free (ctx);
      return NULL;
    }

    if (!SSL_CTX_use_RSAPrivateKey (ctx, rsa)) {
      RSA_free (rsa);
      X509_free (cert);
      SSL_CTX_free (ctx);
      return NULL;
    }
    RSA_free (rsa);

    return cert;
  }

#endif
  return NULL;
}
