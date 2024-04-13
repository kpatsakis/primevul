daemon_linux_lvm2_vg_set_name_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  const gchar *uuid = user_data_elements[0];
  const gchar *new_name = user_data_elements[1];
  const gchar *vg_name;
  guint n;
  gchar *argv[10];

  /* Unfortunately vgchange does not (yet - file a bug) accept UUIDs - so find the VG name for this
   * UUID by looking at PVs
   */

  vg_name = find_lvm2_vg_name_for_uuid (daemon, uuid);
  if (vg_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find VG with UUID `%s'", uuid);
      goto out;
    }

  n = 0;
  argv[n++] = "vgrename";
  argv[n++] = (gchar *) vg_name;
  argv[n++] = (gchar *) new_name;
  argv[n++] = NULL;

  if (!job_new (context, "LinuxLvm2VGSetName", TRUE, NULL, argv, NULL, linux_lvm2_vg_set_name_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
