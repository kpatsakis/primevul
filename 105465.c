device_partition_table_create_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  const char *scheme = user_data_elements[0];
  char **options = user_data_elements[1];
  int n;
  int m;
  char *argv[128];
  GError *error;

  error = NULL;

  if (device_local_is_busy (device, TRUE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  if (strlen (scheme) == 0)
    {
      throw_error (context, ERROR_FAILED, "type not specified");
      goto out;
    }

  n = 0;
  argv[n++] = "udisks-helper-create-partition-table";
  if (device->priv->device_is_linux_dmmp)
    argv[n++] = (gchar *) get_dmmp_device_node (device);
  else
    argv[n++] = device->priv->device_file;
  argv[n++] = (char *) scheme;
  for (m = 0; options[m] != NULL; m++)
    {
      if (n >= (int) sizeof(argv) - 1)
        {
          throw_error (context, ERROR_FAILED, "Too many options");
          goto out;
        }
      /* the helper will validate each option */
      argv[n++] = (char *) options[m];
    }
  argv[n++] = NULL;

  if (!job_new (context,
                "PartitionTableCreate",
                TRUE,
                device,
                argv,
                NULL,
                partition_table_create_completed_cb,
                FALSE,
                partition_table_create_data_new (context, device, scheme),
                (GDestroyNotify) partition_table_create_data_unref))
    {
      goto out;
    }

 out:
  ;
}
