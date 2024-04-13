daemon_linux_lvm2_vg_remove_pv (Daemon *daemon,
                                const gchar *vg_uuid,
                                const gchar *pv_uuid,
                                gchar **options,
                                DBusGMethodInvocation *context)
{
  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2VGRemovePV",
                           TRUE,
                           daemon_linux_lvm2_vg_remove_pv_authorized_cb,
                           context,
                           3,
                           g_strdup (vg_uuid),
                           g_free,
                           g_strdup (pv_uuid),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
