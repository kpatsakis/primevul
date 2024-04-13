str_has_lv_uuid (const gchar *str,
                 const gchar *lv_uuid)
{
  gchar **tokens;
  guint n;

  tokens = g_strsplit (str, ";", 0);
  for (n = 0; tokens != NULL && tokens[n] != NULL; n++)
    {
      if (g_str_has_prefix (tokens[n], "uuid=") && g_strcmp0 (tokens[n] + 5, lv_uuid) == 0)
        {
          return TRUE;
        }
    }

  return FALSE;
}
