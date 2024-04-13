device_filesystem_check (Device *device,
                         char **options,
                         DBusGMethodInvocation *context)
{
  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ?
                           "org.freedesktop.udisks.filesystem-check-system-internal" :
                           "org.freedesktop.udisks.filesystem-check",
                           "FilesystemCheck",
                           TRUE,
                           device_filesystem_check_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);
  return TRUE;
}
