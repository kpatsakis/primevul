dup_list_from_ptrarray (GPtrArray *p)
{
  GList *ret;
  guint n;

  ret = NULL;

  for (n = 0; n < p->len; n++)
    ret = g_list_prepend (ret, g_strdup (((gchar **) p->pdata)[n]));

  return ret;
}
