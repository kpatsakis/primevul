daemon_linux_lvm2_vg_remove_pv_authorized_cb (Daemon *daemon,
                                              Device *device,
                                              DBusGMethodInvocation *context,
                                              const gchar *action_id,
                                              guint num_user_data,
                                              gpointer *user_data_elements)
{
  const gchar *vg_uuid = user_data_elements[0];
  const gchar *pv_uuid = user_data_elements[1];
  /* TODO: use options: gchar **options = user_data_elements[2]; */
  const gchar *vg_name;
  guint n;
  gchar *argv[10];
  Device *pv;
  GError *error;
  GList *devices;
  GList *l;

  /* Unfortunately vgchange does not (yet - file a bug) accept UUIDs - so find the VG name for this
   * UUID by looking at PVs
   */
  pv = NULL;

  vg_name = find_lvm2_vg_name_for_uuid (daemon, vg_uuid);
  if (vg_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find VG with UUID `%s'", vg_uuid);
      goto out;
    }

  devices = daemon_local_get_all_devices (daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);

      if (d->priv->device_is_linux_lvm2_pv && g_strcmp0 (d->priv->linux_lvm2_pv_uuid, pv_uuid) == 0)
        {
          pv = d;
          break;
        }
    }

  if (pv == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find PV with UUID `%s'", pv_uuid);
      goto out;
    }

  error = NULL;
  if (device_local_is_busy (pv, TRUE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  n = 0;
  argv[n++] = "vgreduce";
  argv[n++] = (gchar *) vg_name;
  argv[n++] = (gchar *) pv->priv->device_file;
  argv[n++] = NULL;

  if (!job_new (context, "LinuxLvm2VGRemovePV", TRUE, NULL, argv, NULL, linux_lvm2_vg_remove_pv_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
