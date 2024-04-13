daemon_linux_lvm2_vg_add_pv (Daemon *daemon,
                             const gchar *uuid,
                             const gchar *object_path,
                             gchar **options,
                             DBusGMethodInvocation *context)
{
  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2VGAddPV",
                           TRUE,
                           daemon_linux_lvm2_vg_add_pv_authorized_cb,
                           context,
                           3,
                           g_strdup (uuid),
                           g_free,
                           g_strdup (object_path),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
