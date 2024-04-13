uiserver_decrypt_verify (void *engine, gpgme_data_t ciph, gpgme_data_t plain)
{
  return _uiserver_decrypt (engine, 1, ciph, plain);
}
