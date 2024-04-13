daemon_linux_lvm2_lv_set_name (Daemon *daemon,
                               const gchar *group_uuid,
                               const gchar *uuid,
                               const gchar *new_name,
                               DBusGMethodInvocation *context)
{
  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2LVSetName",
                           TRUE,
                           daemon_linux_lvm2_lv_set_name_authorized_cb,
                           context,
                           3,
                           g_strdup (group_uuid),
                           g_free,
                           g_strdup (uuid),
                           g_free,
                           g_strdup (new_name),
                           g_free);

  return TRUE;
}
