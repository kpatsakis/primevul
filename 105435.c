device_linux_md_add_spare_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  char *component = user_data_elements[0];
  /* TODO: use options */
  int n;
  char *argv[10];
  GError *error;
  Device *slave;

  error = NULL;

  slave = daemon_local_find_by_object_path (device->priv->daemon, component);
  if (slave == NULL)
    {
      throw_error (context, ERROR_FAILED, "Component doesn't exist");
      goto out;
    }

  /* it's fine if the given device isn't a Linux md component _yet_; think
   * hot adding a new disk if an old one failed
   */

  if (device_local_is_busy (slave, TRUE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  /* TODO: check component size is OK */

  n = 0;
  argv[n++] = "mdadm";
  argv[n++] = "--manage";
  argv[n++] = device->priv->device_file;
  argv[n++] = "--add";
  argv[n++] = slave->priv->device_file;
  argv[n++] = "--force";
  argv[n++] = NULL;

  if (!job_new (context,
                "LinuxMdAddSpare",
                TRUE,
                device,
                argv,
                NULL,
                linux_md_add_spare_completed_cb,
                FALSE,
                g_object_ref (slave),
                g_object_unref))
    {
      goto out;
    }

 out:
  ;
}
