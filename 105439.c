device_linux_md_expand_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  gchar **components = user_data_elements[0];
  /* TODO: use options */
  guint n;
  GError *error;
  GPtrArray *args;
  gint new_num_raid_devices;
  gchar *backup_filename;
  gchar *md_basename;

  error = NULL;

  args = g_ptr_array_new_with_free_func (g_free);
  g_ptr_array_add (args, g_strdup ("udisks-helper-mdadm-expand"));
  g_ptr_array_add (args, g_strdup (device->priv->device_file));

  new_num_raid_devices = device->priv->linux_md_num_raid_devices + g_strv_length (components);
  g_ptr_array_add (args, g_strdup_printf ("%d", new_num_raid_devices));

  /* TODO: choose a better name and better location */
  md_basename = g_path_get_basename (device->priv->device_file);
  backup_filename = g_strdup_printf ("/root/udisks-mdadm-expand-backup-file-%s-at-%" G_GUINT64_FORMAT,
                                     md_basename,
                                     (guint64) time (NULL));
  g_free (md_basename);
  g_ptr_array_add (args, backup_filename);

  for (n = 0; components != NULL && components[n] != NULL; n++)
    {
      Device *slave;

      slave = daemon_local_find_by_object_path (device->priv->daemon, components[n]);
      if (slave == NULL)
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Component with object path %s doesn't exist",
                       components[n]);
          goto out;
        }

      if (device_local_is_busy (slave, TRUE, &error))
        {
          dbus_g_method_return_error (context, error);
          g_error_free (error);
          goto out;
        }

      g_ptr_array_add (args, g_strdup (slave->priv->device_file));
    }
  g_ptr_array_add (args, NULL);


  if (!job_new (context,
                "LinuxMdExpand",
                TRUE,
                device,
                (char **) args->pdata,
                NULL,
                linux_md_expand_completed_cb,
                FALSE,
                NULL,
                NULL))
    {
      goto out;
    }

 out:
  g_ptr_array_free (args, TRUE);
}
