make_preamble (opaque * uint64_data, opaque type, uint16_t c_length,
               opaque ver, opaque * preamble)
{
  opaque minor = _gnutls_version_get_minor (ver);
  opaque major = _gnutls_version_get_major (ver);
  opaque *p = preamble;

  memcpy (p, uint64_data, 8);
  p += 8;
  *p = type;
  p++;
  if (_gnutls_version_has_variable_padding (ver))
    {                           /* TLS 1.0 or higher */
      *p = major;
      p++;
      *p = minor;
      p++;
    }
  memcpy (p, &c_length, 2);
  p += 2;
  return p - preamble;
}
