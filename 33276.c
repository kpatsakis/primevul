_gnutls_compressed2ciphertext (gnutls_session_t session,
                               opaque * cipher_data, int cipher_size,
                               gnutls_datum_t compressed,
                               content_type_t _type, int random_pad,
                               record_parameters_st * params)
{
  uint8_t MAC[MAX_HASH_SIZE];
  uint16_t c_length;
  uint8_t pad;
  int length, ret;
  uint8_t type = _type;
  opaque preamble[PREAMBLE_SIZE];
  int preamble_size;
  int hash_size = _gnutls_hash_get_algo_len (params->mac_algorithm);
  int blocksize = gnutls_cipher_get_block_size (params->cipher_algorithm);
  cipher_type_t block_algo =
    _gnutls_cipher_is_block (params->cipher_algorithm);
  opaque *data_ptr;
  int ver = gnutls_protocol_get_version (session);


  /* Initialize MAC */

  c_length = _gnutls_conv_uint16 (compressed.size);

  if (params->mac_algorithm != GNUTLS_MAC_NULL)
    {                           /* actually when the algorithm in not the NULL one */
      digest_hd_st td;

      ret = mac_init (&td, params->mac_algorithm,
                      params->write.mac_secret.data,
                      params->write.mac_secret.size, ver);

      if (ret < 0)
        {
          gnutls_assert ();
          return ret;
        }
      preamble_size =
        make_preamble (UINT64DATA
                       (params->write.sequence_number),
                       type, c_length, ver, preamble);
      mac_hash (&td, preamble, preamble_size, ver);
      mac_hash (&td, compressed.data, compressed.size, ver);
      mac_deinit (&td, MAC, ver);
    }


  /* Calculate the encrypted length (padding etc.)
   */
  length =
    calc_enc_length (session, compressed.size, hash_size, &pad,
                     random_pad, block_algo, blocksize);
  if (length < 0)
    {
      gnutls_assert ();
      return length;
    }

  /* copy the encrypted data to cipher_data.
   */
  if (cipher_size < length)
    {
      gnutls_assert ();
      return GNUTLS_E_MEMORY_ERROR;
    }

  data_ptr = cipher_data;
  if (block_algo == CIPHER_BLOCK &&
      _gnutls_version_has_explicit_iv (session->security_parameters.version))
    {
      /* copy the random IV.
       */
      ret = _gnutls_rnd (GNUTLS_RND_NONCE, data_ptr, blocksize);
      if (ret < 0)
        {
          gnutls_assert ();
          return ret;
        }

      data_ptr += blocksize;
    }

  memcpy (data_ptr, compressed.data, compressed.size);
  data_ptr += compressed.size;

  if (hash_size > 0)
    {
      memcpy (data_ptr, MAC, hash_size);
      data_ptr += hash_size;
    }
  if (block_algo == CIPHER_BLOCK && pad > 0)
    {
      memset (data_ptr, pad - 1, pad);
    }


  /* Actual encryption (inplace).
   */
  ret =
    _gnutls_cipher_encrypt (&params->write.cipher_state, cipher_data, length);
  if (ret < 0)
    {
      gnutls_assert ();
      return ret;
    }

  return length;
}
