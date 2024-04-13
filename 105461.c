device_partition_delete (Device *device,
                         char **options,
                         DBusGMethodInvocation *context)
{
  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "PartitionDelete",
                           TRUE,
                           device_partition_delete_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);
  return TRUE;
}
