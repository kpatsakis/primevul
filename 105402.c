device_drive_benchmark_authorized_cb (Daemon *daemon,
                                      Device *device,
                                      DBusGMethodInvocation *context,
                                      const gchar *action_id,
                                      guint num_user_data,
                                      gpointer *user_data_elements)
{
  gboolean do_write_benchmark = GPOINTER_TO_INT (user_data_elements[0]);
  gchar **options = user_data_elements[1];
  gchar *argv[16];
  guint n;

  if (!device->priv->device_is_drive)
    {
      throw_error (context, ERROR_FAILED, "Device is not a drive");
      goto out;
    }

  if (do_write_benchmark)
    {
      if (device->priv->device_is_partition_table)
        {
          throw_error (context, ERROR_FAILED, "A partition table was detected - write benchmarking requires "
                       "the disk to be completely empty");
          goto out;
        }

      if (device->priv->id_usage != NULL)
        {
          throw_error (context, ERROR_FAILED, "The disk seems to have usage `%s' - write benchmarking requires "
                       "the disk to be completely empty", device->priv->id_usage);
          goto out;
        }
    }

  for (n = 0; options[n] != NULL; n++)
    {
      const char *option = options[n];
      throw_error (context, ERROR_INVALID_OPTION, "Unknown option %s", option);
      goto out;
    }

  n = 0;
  argv[n++] = "udisks-helper-drive-benchmark";
  argv[n++] = device->priv->device_file;
  argv[n++] = do_write_benchmark ? "1" : "0";
  argv[n++] = NULL;

  if (!job_new (context, "DriveBenchmark", TRUE, device, argv, NULL, drive_benchmark_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
