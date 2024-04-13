daemon_linux_lvm2_lv_set_name_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  const gchar *group_uuid = user_data_elements[0];
  const gchar *uuid = user_data_elements[1];
  const gchar *new_name = user_data_elements[2];
  const gchar *vg_name;
  gchar *lv_name;
  guint n;
  gchar *argv[10];

  /* Unfortunately lvchange does not (yet - file a bug) accept UUIDs - so find the LV name for this
   * UUID by looking at PVs
   */

  lv_name = NULL;

  vg_name = find_lvm2_vg_name_for_uuid (daemon, group_uuid);
  if (vg_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find VG with UUID `%s'", group_uuid);
      goto out;
    }

  lv_name = find_lvm2_lv_name_for_uuids (daemon, group_uuid, uuid);
  if (lv_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find LV with UUID `%s'", uuid);
      goto out;
    }

  n = 0;
  argv[n++] = "lvrename";
  argv[n++] = (gchar *) vg_name;
  argv[n++] = lv_name;
  argv[n++] = (gchar *) new_name;
  argv[n++] = NULL;

  if (!job_new (context, "LinuxLvm2LVSetName", TRUE, NULL, argv, NULL, linux_lvm2_lv_set_name_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  g_free (lv_name);
}
