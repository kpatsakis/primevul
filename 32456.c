stringprep_utf8_to_ucs4 (const char *str, ssize_t len, size_t * items_written)
{
  size_t n;

  if (len < 0)
    n = strlen (str);
  else
    n = len;

  if (u8_check ((const uint8_t *) str, n))
    return NULL;

  return g_utf8_to_ucs4_fast (str, (glong) len, (glong *) items_written);
}
