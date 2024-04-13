find_lvm2_vg_name_for_uuid (Daemon *daemon,
                            const gchar *uuid)
{
  GList *l;
  GList *devices;
  const gchar *vg_name;

  vg_name = NULL;

  devices = daemon_local_get_all_devices (daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);

      if (d->priv->device_is_linux_lvm2_pv && g_strcmp0 (uuid, d->priv->linux_lvm2_pv_group_uuid) == 0)
        {
          vg_name = d->priv->linux_lvm2_pv_group_name;
          break;
        }
    }
  g_list_free (devices);

  return vg_name;
}
