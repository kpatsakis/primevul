_dupv8 (const char *s)
{
  const char *end_valid;

  if (!g_utf8_validate (s, -1, &end_valid))
    {
      g_print ("**** NOTE: The string '%s' is not valid UTF-8. Invalid characters begins at '%s'\n", s, end_valid);
      return g_strndup (s, end_valid - s);
    }
  else
    {
      return g_strdup (s);
    }
}
