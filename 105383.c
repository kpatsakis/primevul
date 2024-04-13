daemon_linux_lvm2_vg_set_name (Daemon *daemon,
                               const gchar *uuid,
                               const gchar *new_name,
                               DBusGMethodInvocation *context)
{
  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2VGSetName",
                           TRUE,
                           daemon_linux_lvm2_vg_set_name_authorized_cb,
                           context,
                           2,
                           g_strdup (uuid),
                           g_free,
                           g_strdup (new_name),
                           g_free);

  return TRUE;
}
