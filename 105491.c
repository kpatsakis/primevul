find_lvm2_lv_name_for_uuids (Daemon *daemon,
                             const gchar *group_uuid,
                             const gchar *uuid)
{
  GList *l;
  GList *devices;
  gchar *ret;

  ret = NULL;

  devices = daemon_local_get_all_devices (daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);

      if (d->priv->device_is_linux_lvm2_pv &&
          g_strcmp0 (group_uuid, d->priv->linux_lvm2_pv_group_uuid) == 0)
        {
          guint n;

          for (n = 0; n < d->priv->linux_lvm2_pv_group_logical_volumes->len; n++)
            {
              const gchar *lv_data = d->priv->linux_lvm2_pv_group_logical_volumes->pdata[n];
              gchar **tokens;
              guint m;

              tokens = g_strsplit (lv_data, ";", 0);
              for (m = 0; tokens != NULL && tokens[m] != NULL; m++)
                {
                  if (g_str_has_prefix (tokens[m], "uuid=") && g_strcmp0 (tokens[m] + 5, uuid) == 0)
                    {
                      guint c;
                      for (c = 0; tokens[c] != NULL; c++)
                        {
                          if (g_str_has_prefix (tokens[c], "name="))
                            {
                              ret = g_strdup_printf ("%s/%s",
                                                     d->priv->linux_lvm2_pv_group_name,
                                                     tokens[c] + 5);
                              break;
                            }
                        }
                    }
                }
              g_strfreev (tokens);

              if (ret != NULL)
                break;
            }
        }
      if (ret != NULL)
        break;
    }
  g_list_free (devices);

  return ret;
}
