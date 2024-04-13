device_filesystem_create (Device *device,
                          const char *fstype,
                          char **options,
                          DBusGMethodInvocation *context)
{
  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "FilesystemCreate",
                           TRUE,
                           device_filesystem_create_authorized_cb,
                           context,
                           2,
                           g_strdup (fstype),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);
  return TRUE;
}
