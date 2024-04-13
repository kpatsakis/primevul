uiserver_decrypt (void *engine, gpgme_data_t ciph, gpgme_data_t plain)
{
  return _uiserver_decrypt (engine, 0, ciph, plain);
}
