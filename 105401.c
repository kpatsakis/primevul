device_drive_benchmark (Device *device,
                        gboolean do_write_benchmark,
                        char **options,
                        DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.change",
                           "DriveBenchmark",
                           TRUE,
                           device_drive_benchmark_authorized_cb,
                           context,
                           2,
                           GINT_TO_POINTER (do_write_benchmark),
                           NULL,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}
