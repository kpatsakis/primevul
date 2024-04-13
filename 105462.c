device_partition_modify (Device *device,
                         const char *type,
                         const char *label,
                         char **flags,
                         DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_partition)
    {
      throw_error (context, ERROR_FAILED, "Device is not a partition");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "PartitionModify",
                           TRUE,
                           device_partition_modify_authorized_cb,
                           context,
                           3,
                           g_strdup (type),
                           g_free,
                           g_strdup (label),
                           g_free,
                           g_strdupv (flags),
                           g_strfreev);

 out:
  return TRUE;
}
