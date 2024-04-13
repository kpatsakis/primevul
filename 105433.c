device_linux_lvm2_lv_stop (Device *device,
                           char **options,
                           DBusGMethodInvocation *context)
{
  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.linux-lvm2",
                           "LinuxLvm2LVStop",
                           TRUE,
                           daemon_linux_lvm2_lv_stop_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
