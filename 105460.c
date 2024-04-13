device_partition_create_authorized_cb (Daemon *daemon,
                                       Device *device,
                                       DBusGMethodInvocation *context,
                                       const gchar *action_id,
                                       guint num_user_data,
                                       gpointer *user_data_elements)
{
  guint64 offset = *((guint64*) user_data_elements[0]);
  guint64 size = *((guint64*) user_data_elements[1]);
  ;
  const char *type = user_data_elements[2];
  const char *label = user_data_elements[3];
  char **flags = user_data_elements[4];
  char **options = user_data_elements[5];
  const char *fstype = user_data_elements[6];
  char **fsoptions = user_data_elements[7];
  int n;
  int m;
  char *argv[128];
  GError *error;
  char *offset_as_string;
  char *size_as_string;
  char *flags_as_string;

  offset_as_string = NULL;
  size_as_string = NULL;
  flags_as_string = NULL;
  error = NULL;

  if (!device->priv->device_is_partition_table)
    {
      throw_error (context, ERROR_FAILED, "Device is not partitioned");
      goto out;
    }

  if (device_local_is_busy (device, FALSE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  /* partutil.c / libparted will check there are no partitions in the requested slice */

  offset_as_string = g_strdup_printf ("%" G_GINT64_FORMAT "", offset);
  size_as_string = g_strdup_printf ("%" G_GINT64_FORMAT "", size);
  /* TODO: check that neither of the flags include ',' */
  flags_as_string = g_strjoinv (",", flags);

  n = 0;
  argv[n++] = "udisks-helper-create-partition";
  if (device->priv->device_is_linux_dmmp)
    argv[n++] = (gchar *) get_dmmp_device_node (device);
  else
    argv[n++] = device->priv->device_file;
  argv[n++] = offset_as_string;
  argv[n++] = size_as_string;
  argv[n++] = (char *) type;
  argv[n++] = (char *) label;
  argv[n++] = (char *) flags_as_string;
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
                "PartitionCreate",
                TRUE,
                device,
                argv,
                NULL,
                partition_create_completed_cb,
                FALSE,
                partition_create_data_new (context, device, offset, size, fstype, fsoptions),
                (GDestroyNotify) partition_create_data_unref))
    {
      goto out;
    }

 out:
  g_free (offset_as_string);
  g_free (size_as_string);
  g_free (flags_as_string);
}
