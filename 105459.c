device_partition_create (Device *device,
                         guint64 offset,
                         guint64 size,
                         const char *type,
                         const char *label,
                         char **flags,
                         char **options,
                         const char *fstype,
                         char **fsoptions,
                         DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_partition_table)
    {
      throw_error (context, ERROR_FAILED, "Device is not partitioned");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "PartitionCreate",
                           TRUE,
                           device_partition_create_authorized_cb,
                           context,
                           8,
                           g_memdup (&offset, sizeof(guint64)),
                           g_free,
                           g_memdup (&size, sizeof(guint64)),
                           g_free,
                           g_strdup (type),
                           g_free,
                           g_strdup (label),
                           g_free,
                           g_strdupv (flags),
                           g_strfreev,
                           g_strdupv (options),
                           g_strfreev,
                           g_strdup (fstype),
                           g_free,
                           g_strdupv (fsoptions),
                           g_strfreev);

 out:
  return TRUE;
}
