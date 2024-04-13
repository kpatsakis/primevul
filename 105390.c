daemon_linux_md_create_authorized_cb (Daemon *daemon,
                                      Device *device,
                                      DBusGMethodInvocation *context,
                                      const gchar *action_id,
                                      guint num_user_data,
                                      gpointer *user_data_elements)
{
  gchar **components_as_strv = user_data_elements[0];
  gchar *level = user_data_elements[1];
  guint64 stripe_size = *((guint64*) user_data_elements[2]);
  gchar *name = user_data_elements[3];
  /* TODO: use options */
  int n;
  int m;
  char *argv[128];
  GError *error;
  gchar *md_device_file;
  gchar *num_raid_devices_as_str;
  gchar *stripe_size_as_str;
  gboolean use_bitmap;
  gboolean use_chunk;

  md_device_file = NULL;
  num_raid_devices_as_str = NULL;
  stripe_size_as_str = NULL;
  error = NULL;

  /* sanity-check level */
  use_bitmap = FALSE;
  use_chunk = FALSE;
  if (g_strcmp0 (level, "raid0") == 0)
    {
      use_chunk = TRUE;
    }
  else if (g_strcmp0 (level, "raid1") == 0)
    {
      if (stripe_size > 0)
        {
          throw_error (context, ERROR_FAILED, "Stripe size doesn't make sense for RAID-1");
          goto out;
        }
    }
  else if (g_strcmp0 (level, "raid4") == 0 || g_strcmp0 (level, "raid5") == 0 || g_strcmp0 (level, "raid6") == 0
           || g_strcmp0 (level, "raid10") == 0)
    {
      use_bitmap = TRUE;
      use_chunk = TRUE;
    }
  else
    {
      throw_error (context, ERROR_FAILED, "Invalid level `%s'", level);
      goto out;
    }

  /* check that all given components exist and that they are not busy
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

  num_raid_devices_as_str = g_strdup_printf ("%d", g_strv_length (components_as_strv));

  if (stripe_size > 0)
    stripe_size_as_str = g_strdup_printf ("%d", ((gint) stripe_size) / 1024);

  n = 0;
  argv[n++] = "mdadm";
  argv[n++] = "--create";
  argv[n++] = md_device_file;
  argv[n++] = "--level";
  argv[n++] = level;
  argv[n++] = "--raid-devices";
  argv[n++] = num_raid_devices_as_str;
  argv[n++] = "--metadata";
  argv[n++] = "1.2";
  argv[n++] = "--name";
  argv[n++] = name;
  argv[n++] = "--homehost";
  argv[n++] = "";
  if (use_bitmap)
    {
      argv[n++] = "--bitmap";
      argv[n++] = "internal";
    }
  if (use_chunk && stripe_size_as_str != NULL)
    {
      argv[n++] = "--chunk";
      argv[n++] = stripe_size_as_str;
    }
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
                "LinuxMdCreate",
                TRUE,
                NULL,
                argv,
                NULL,
                linux_md_create_completed_cb,
                FALSE,
                linux_md_create_data_new (context, daemon, components_as_strv[0]),
                (GDestroyNotify) linux_md_create_data_unref))
    {
      goto out;
    }

 out:
  g_free (md_device_file);
  g_free (num_raid_devices_as_str);
  g_free (stripe_size_as_str);
}
