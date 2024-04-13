daemon_linux_lvm2_lv_start_authorized_cb (Daemon *daemon,
                                          Device *device,
                                          DBusGMethodInvocation *context,
                                          const gchar *action_id,
                                          guint num_user_data,
                                          gpointer *user_data_elements)
{
  const gchar *group_uuid = user_data_elements[0];
  const gchar *uuid = user_data_elements[1];
  gchar *lv_name;
  /* TODO: use options */
  guint n;
  gchar *argv[10];

  /* Unfortunately vgchange does not (yet - file a bug) accept UUIDs - so find the VG name for this
   * UUID by looking at PVs
   */

  lv_name = find_lvm2_lv_name_for_uuids (daemon, group_uuid, uuid);
  if (lv_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find Logical Volume with Group UUID `%s' and UUID `%s'", group_uuid, uuid);
      goto out;
    }

  n = 0;
  argv[n++] = "lvchange";
  argv[n++] = "-ay";
  argv[n++] = lv_name;
  argv[n++] = NULL;

  if (!job_new (context, "LinuxLvm2LVStart", TRUE, NULL, argv, NULL, linux_lvm2_lv_start_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  g_free (lv_name);
}
