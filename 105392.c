daemon_linux_md_start_authorized_cb (Daemon *daemon,
                                     Device *device,
                                     DBusGMethodInvocation *context,
                                     const gchar *action_id,
                                     guint num_user_data,
                                     gpointer *user_data_elements)
{
  gchar **components_as_strv = user_data_elements[0];
  /* TODO: use options */
  int n;
  int m;
  char *argv[128];
  GError *error;
  char *uuid;
  char *md_device_file;

  uuid = NULL;
  md_device_file = NULL;
  error = NULL;

  /* check that all given components exist, that they are indeed linux-md-components and
   * that their uuid agrees
   */
  for (n = 0; components_as_strv[n] != NULL; n++)
    {
      Device *slave;
      const char *component_objpath = components_as_strv[n];

      slave = daemon_local_find_by_object_path (daemon, component_objpath);
      if (slave == NULL)
        {
          throw_error (context, ERROR_FAILED, "Component %s doesn't exist", component_objpath);
          goto out;
        }

      if (!slave->priv->device_is_linux_md_component)
        {
          throw_error (context, ERROR_FAILED, "%s is not a linux-md component", component_objpath);
          goto out;
        }

      if (n == 0)
        {
          uuid = g_strdup (slave->priv->linux_md_component_uuid);
          if (uuid == NULL)
            {
              throw_error (context, ERROR_FAILED, "no uuid for one of the components");
              goto out;
            }
        }
      else
        {
          const char *this_uuid;
          this_uuid = slave->priv->linux_md_component_uuid;

          if (this_uuid == NULL || strcmp (uuid, this_uuid) != 0)
            {
              throw_error (context, ERROR_FAILED, "uuid mismatch between given components");
              goto out;
            }
        }

      if (device_local_is_busy (slave, FALSE, &error))
        {
          dbus_g_method_return_error (context, error);
          g_error_free (error);
          goto out;
        }
    }

  /* find an unused md minor... Man, I wish mdadm could do this itself; this is slightly racy */
  for (n = 0; TRUE; n++)
    {
      char *native_path;
      char *array_state;

      /* TODO: move to /sys/class/block instead */
      native_path = g_strdup_printf ("/sys/block/md%d", n);
      if (!sysfs_file_exists (native_path, "md/array_state"))
        {
          /* Apparently this slot is free since there is no such file. So let's peruse it. */
          g_free (native_path);
          break;
        }
      else
        {
          array_state = sysfs_get_string (native_path, "md/array_state");
          g_strstrip (array_state);
          if (strcmp (array_state, "clear") == 0)
            {
              /* It's clear! Let's use it! */
              g_free (array_state);
              g_free (native_path);
              break;
            }
          g_free (array_state);
        }
      g_free (native_path);
    }

  md_device_file = g_strdup_printf ("/dev/md%d", n);

  n = 0;
  argv[n++] = "mdadm";
  argv[n++] = "--assemble";
  argv[n++] = md_device_file;
  argv[n++] = "--run";
  for (m = 0; components_as_strv[m] != NULL; m++)
    {
      Device *slave;
      const char *component_objpath = components_as_strv[m];

      slave = daemon_local_find_by_object_path (daemon, component_objpath);
      if (slave == NULL)
        {
          throw_error (context, ERROR_FAILED, "Component %s doesn't exist", component_objpath);
          goto out;
        }

      if (n >= (int) sizeof(argv) - 1)
        {
          throw_error (context, ERROR_FAILED, "Too many components");
          goto out;
        }

      argv[n++] = (char *) slave->priv->device_file;
    }
  argv[n++] = NULL;

  if (!job_new (context,
                "LinuxMdStart",
                TRUE,
                NULL,
                argv,
                NULL,
                linux_md_start_completed_cb,
                FALSE,
                linux_md_start_data_new (context, daemon, uuid),
                (GDestroyNotify) linux_md_start_data_unref))
    {
      goto out;
    }

 out:
  g_free (uuid);
  g_free (md_device_file);
}
