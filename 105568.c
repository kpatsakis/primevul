lvm2_lv_create_has_lv (CreateLvm2LVData *data)
{
  GList *devices;
  Device *ret;
  GList *l;

  ret = NULL;

  devices = daemon_local_get_all_devices (data->daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);
      if (d->priv->device_is_linux_lvm2_lv &&
          g_strcmp0 (d->priv->linux_lvm2_lv_group_uuid, data->vg_uuid) == 0 &&
          g_strcmp0 (d->priv->linux_lvm2_lv_name, data->lv_name) == 0)
        {
          GList *m;
          const gchar *lv_uuid;

          lv_uuid = d->priv->linux_lvm2_lv_uuid;

          /* OK, we've found the LV... now check that one of more PVs actually reference this LV */
          for (m = devices; m != NULL; m = m->next)
            {
              Device *pv = DEVICE (m->data);
              if (pv->priv->device_is_linux_lvm2_pv &&
                  g_strcmp0 (pv->priv->linux_lvm2_pv_group_uuid, data->vg_uuid) == 0)
                {
                  guint n;
                  for (n = 0; n < pv->priv->linux_lvm2_pv_group_logical_volumes->len; n++)
                    {
                      const gchar *str = pv->priv->linux_lvm2_pv_group_logical_volumes->pdata[n];
                      if (str_has_lv_uuid (str, lv_uuid))
                        {
                          /* Return the LV, not the PV */
                          ret = d;
                          break;
                        }
                    }
                }
            } /* for all PVs */

          break;
        } /* if (found LV) */
    }
  return ret;
}
