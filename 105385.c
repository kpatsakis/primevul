daemon_linux_lvm2_vg_start (Daemon *daemon,
                            const gchar *uuid,
                            char **options,
                            DBusGMethodInvocation *context)
{
  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2VGStart",
                           TRUE,
                           daemon_linux_lvm2_vg_start_authorized_cb,
                           context,
                           2,
                           g_strdup (uuid),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
