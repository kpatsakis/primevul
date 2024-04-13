_tls_add_certificates (SSL_CTX * ctx)
{
  int count = 0;

#ifdef HAVE_WINCRYPT_H
  PCCERT_CONTEXT pCertCtx;
  X509 *cert = NULL;
  HCERTSTORE hStore = CertOpenSystemStore (0, L"CA");

  for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
    cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
    if (cert == NULL) {
      continue;
    }
    /*tls_dump_cert_info("CA", cert); */

    if (!X509_STORE_add_cert (ctx->cert_store, cert)) {
      X509_free (cert);
      continue;
    }
    count++;
    X509_free (cert);
  }

  CertCloseStore (hStore, 0);

  hStore = CertOpenSystemStore (0, L"ROOT");

  for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
    cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
    if (cert == NULL) {
      continue;
    }
    /*tls_dump_cert_info("ROOT", cert); */

    if (!X509_STORE_add_cert (ctx->cert_store, cert)) {
      X509_free (cert);
      continue;
    }
    count++;
    X509_free (cert);
  }

  CertCloseStore (hStore, 0);

  hStore = CertOpenSystemStore (0, L"MY");

  for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
    cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
    if (cert == NULL) {
      continue;
    }
    /*tls_dump_cert_info("MY", cert); */

    if (!X509_STORE_add_cert (ctx->cert_store, cert)) {
      X509_free (cert);
      continue;
    }
    count++;
    X509_free (cert);
  }

  CertCloseStore (hStore, 0);

  hStore = CertOpenSystemStore (0, L"Trustedpublisher");

  for (pCertCtx = CertEnumCertificatesInStore (hStore, NULL); pCertCtx != NULL; pCertCtx = CertEnumCertificatesInStore (hStore, pCertCtx)) {
    cert = d2i_X509 (NULL, (const unsigned char **) &pCertCtx->pbCertEncoded, pCertCtx->cbCertEncoded);
    if (cert == NULL) {
      continue;
    }
    /*tls_dump_cert_info("Trustedpublisher", cert); */

    if (!X509_STORE_add_cert (ctx->cert_store, cert)) {
      X509_free (cert);
      continue;
    }
    count++;
    X509_free (cert);
  }

  CertCloseStore (hStore, 0);
#elif defined(__APPLE__) && (TARGET_OS_IPHONE==0)
  SecKeychainSearchRef pSecKeychainSearch = NULL;
  SecKeychainRef pSecKeychain;
  OSStatus status = noErr;
  X509 *cert = NULL;
  SInt32 osx_version = 0;

  if (Gestalt (gestaltSystemVersion, &osx_version) != noErr) {
    OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "macosx certificate store: can't get osx version"));
    return 0;
  }
  if (osx_version >= 0x1050) {
    /* Leopard store location */
    status = SecKeychainOpen ("/System/Library/Keychains/SystemRootCertificates.keychain", &pSecKeychain);
  }
  else {
    /* Tiger and below store location */
    status = SecKeychainOpen ("/System/Library/Keychains/X509Anchors", &pSecKeychain);
  }
  if (status != noErr) {
    OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "macosx certificate store: can't get osx version"));
    return 0;
  }

  status = SecKeychainSearchCreateFromAttributes (pSecKeychain, kSecCertificateItemClass, NULL, &pSecKeychainSearch);
  for (;;) {
    SecKeychainItemRef pSecKeychainItem = nil;

    status = SecKeychainSearchCopyNext (pSecKeychainSearch, &pSecKeychainItem);
    if (status == errSecItemNotFound) {
      break;
    }

    if (status == noErr) {
      void *_pCertData;
      UInt32 _pCertLength;

      status = SecKeychainItemCopyAttributesAndData (pSecKeychainItem, NULL, NULL, NULL, &_pCertLength, &_pCertData);

      if (status == noErr && _pCertData != NULL) {
        unsigned char *ptr;

        ptr = _pCertData;       /*required because d2i_X509 is modifying pointer */
        cert = d2i_X509 (NULL, (const unsigned char **) &ptr, _pCertLength);
        if (cert == NULL) {
          continue;
        }
        /*tls_dump_cert_info("ROOT", cert); */

        if (!X509_STORE_add_cert (ctx->cert_store, cert)) {
          X509_free (cert);
          continue;
        }
        count++;
        X509_free (cert);

        status = SecKeychainItemFreeAttributesAndData (NULL, _pCertData);
      }
    }

    if (pSecKeychainItem != NULL)
      CFRelease (pSecKeychainItem);

    if (status != noErr) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "macosx certificate store: can't add certificate (%i)", status));
    }
  }

  CFRelease (pSecKeychainSearch);
  CFRelease (pSecKeychain);

#endif
  return count;
}
