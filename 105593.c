ptr_array_has_string (GPtrArray *p, const gchar *str)
{
  guint n;
  gboolean ret;

  ret = FALSE;
  for (n = 0; n < p->len; n++)
    {
      if (g_strcmp0 (p->pdata[n], str) == 0)
        {
          ret = TRUE;
          goto out;
        }
    }

 out:
  return ret;
}
