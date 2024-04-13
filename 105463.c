device_partition_modify_authorized_cb (Daemon *daemon,
                                       Device *device,
                                       DBusGMethodInvocation *context,
                                       const gchar *action_id,
                                       guint num_user_data,
                                       gpointer *user_data_elements)
{
  const char *type = user_data_elements[0];
  const char *label = user_data_elements[1];
  char **flags = user_data_elements[2];
  int n;
  char *argv[128];
  GError *error;
  char *offset_as_string;
  char *size_as_string;
  char *flags_as_string;
  Device *enclosing_device;

  offset_as_string = NULL;
  size_as_string = NULL;
  flags_as_string = NULL;
  error = NULL;

  if (!device->priv->device_is_partition)
    {
      throw_error (context, ERROR_FAILED, "Device is not a partition");
      goto out;
    }

  enclosing_device = daemon_local_find_by_object_path (device->priv->daemon, device->priv->partition_slave);
  if (enclosing_device == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find enclosing device");
      goto out;
    }

  if (device_local_is_busy (enclosing_device, FALSE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  if (strlen (type) == 0)
    {
      throw_error (context, ERROR_FAILED, "type not specified");
      goto out;
    }

  offset_as_string = g_strdup_printf ("%" G_GINT64_FORMAT "", device->priv->partition_offset);
  size_as_string = g_strdup_printf ("%" G_GINT64_FORMAT "", device->priv->partition_size);
  /* TODO: check that neither of the flags include ',' */
  flags_as_string = g_strjoinv (",", flags);

  n = 0;
  argv[n++] = "udisks-helper-modify-partition";
  argv[n++] = enclosing_device->priv->device_file;
  argv[n++] = offset_as_string;
  argv[n++] = size_as_string;
  argv[n++] = (char *) type;
  argv[n++] = (char *) label;
  argv[n++] = (char *) flags_as_string;
  argv[n++] = NULL;

  if (!job_new (context,
                "PartitionModify",
                TRUE,
                device,
                argv,
                NULL,
                partition_modify_completed_cb,
                FALSE,
                partition_modify_data_new (context, device, enclosing_device, type, label, flags),
                (GDestroyNotify) partition_modify_data_unref))
    {
      goto out;
    }

 out:
  g_free (offset_as_string);
  g_free (size_as_string);
  g_free (flags_as_string);
}
