_dbus_generate_random_bytes (DBusString *str,
                             int         n_bytes)
{
  int old_len;
  char *p;
  HCRYPTPROV hprov;

  old_len = _dbus_string_get_length (str);

  if (!_dbus_string_lengthen (str, n_bytes))
    return FALSE;

  p = _dbus_string_get_data_len (str, old_len, n_bytes);

  if (!CryptAcquireContext (&hprov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    return FALSE;

  if (!CryptGenRandom (hprov, n_bytes, p))
    {
      CryptReleaseContext (hprov, 0);
      return FALSE;
    }

  CryptReleaseContext (hprov, 0);

  return TRUE;
}
