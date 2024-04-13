device_partition_table_create (Device *device,
                               const char *scheme,
                               char **options,
                               DBusGMethodInvocation *context)
{
  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "PartitionTableCreate",
                           TRUE,
                           device_partition_table_create_authorized_cb,
                           context,
                           2,
                           g_strdup (scheme),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);
  return TRUE;
}
