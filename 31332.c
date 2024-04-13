tristring_equal (gconstpointer  a,
                 gconstpointer  b)
{
  const char *ap = a;
  const char *bp = b;
  size_t len;

  if (!strequal_len (ap, bp, &len))
    return FALSE;

  ap += len + 1;
  bp += len + 1;

  if (!strequal_len (ap, bp, &len))
    return FALSE;

  ap += len + 1;
  bp += len + 1;

  if (strcmp (ap, bp) != 0)
    return FALSE;
  
  return TRUE;
}
