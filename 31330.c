strequal_len (const char *a,
              const char *b,
              size_t     *lenp)
{
  size_t a_len;
  size_t b_len;

  a_len = strlen (a);
  b_len = strlen (b);

  if (a_len != b_len)
    return FALSE;

  if (memcmp (a, b, a_len) != 0)
    return FALSE;
  
  *lenp = a_len;

  return TRUE;
}
