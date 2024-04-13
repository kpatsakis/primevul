daemon_linux_lvm2_lv_start (Daemon *daemon,
                            const gchar *group_uuid,
                            const gchar *uuid,
                            char **options,
                            DBusGMethodInvocation *context)
{
  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2LVStart",
                           TRUE,
                           daemon_linux_lvm2_lv_start_authorized_cb,
                           context,
                           3,
                           g_strdup (group_uuid),
                           g_free,
                           g_strdup (uuid),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
