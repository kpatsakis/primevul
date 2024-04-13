extract_lvm_uuid (const gchar *s)
{
  GString *str;

  if (s == NULL || strlen (s) < 32)
    return NULL;

  str = g_string_new_len (s, 6);   g_string_append_c (str, '-'); s += 6;
  g_string_append_len (str, s, 4); g_string_append_c (str, '-'); s += 4;
  g_string_append_len (str, s, 4); g_string_append_c (str, '-'); s += 4;
  g_string_append_len (str, s, 4); g_string_append_c (str, '-'); s += 4;
  g_string_append_len (str, s, 4); g_string_append_c (str, '-'); s += 4;
  g_string_append_len (str, s, 4); g_string_append_c (str, '-'); s += 4;
  g_string_append_len (str, s, 6);

  return g_string_free (str, FALSE);
}
