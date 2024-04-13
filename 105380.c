daemon_linux_lvm2_vg_add_pv_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  const gchar *uuid = user_data_elements[0];
  const gchar *physical_volume = user_data_elements[1];
  /* TODO: use options: gchar **options = user_data_elements[2]; */
  const gchar *vg_name;
  guint n;
  gchar *argv[10];
  Device *pv;
  GError *error;

  /* Unfortunately vgchange does not (yet - file a bug) accept UUIDs - so find the VG name for this
   * UUID by looking at PVs
   */

  vg_name = find_lvm2_vg_name_for_uuid (daemon, uuid);
  if (vg_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find VG with UUID `%s'", uuid);
      goto out;
    }

  pv = daemon_local_find_by_object_path (daemon, physical_volume);
  if (pv == NULL)
    {
      throw_error (context, ERROR_FAILED, "physical volume doesn't exist");
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
  argv[n++] = "vgextend";
  argv[n++] = (gchar *) vg_name;
  argv[n++] = (gchar *) pv->priv->device_file;
  argv[n++] = NULL;

  if (!job_new (context, "LinuxLvm2VGAddPV", TRUE, NULL, argv, NULL, linux_lvm2_vg_add_pv_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
