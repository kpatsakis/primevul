mac_hash (digest_hd_st * td, void *data, int data_size, int ver)
{
  if (ver == GNUTLS_SSL3)
    {                           /* SSL 3.0 */
      _gnutls_hash (td, data, data_size);
    }
  else
    {
      _gnutls_hmac (td, data, data_size);
    }
}
