device_filesystem_mount_authorized_cb (Daemon *daemon,
                                       Device *device,
                                       DBusGMethodInvocation *context,
                                       const gchar *action_id,
                                       guint num_user_data,
                                       gpointer *user_data_elements)
{
  const gchar *filesystem_type = user_data_elements[0];
  gchar **given_options = user_data_elements[1];
  int n;
  GString *s;
  char *argv[10];
  char *mount_point;
  char *fstype;
  char *mount_options;
  GError *error;
  uid_t caller_uid;
  gboolean remove_dir_on_unmount;
  const FSMountOptions *fsmo;
  char **options;
  char uid_buf[32];

  fstype = NULL;
  options = NULL;
  mount_options = NULL;
  mount_point = NULL;
  remove_dir_on_unmount = FALSE;
  error = NULL;

  daemon_local_get_uid (device->priv->daemon, &caller_uid, context);

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "filesystem") != 0)
    {
      if ((g_strcmp0 (filesystem_type, "auto") == 0 || g_strcmp0 (filesystem_type, "") == 0) && device->priv->id_usage
          == NULL)
        {
          /* if we don't know the usage of the device and 'auto' or '' is passed for fstype
           * then just try that.. this is to make, for example, mounting /dev/fd0 work (we
           * don't probe such devices for filesystems in udev)
           */
        }
      else
        {
          throw_error (context, ERROR_FAILED, "Not a mountable file system");
          goto out;
        }
    }

  if (device_local_is_busy (device, FALSE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  /* Check if the device is referenced in /etc/fstab; if so, attempt to
   * mount the device as the user
   */
  if (is_device_in_fstab (device, &mount_point))
    {
      n = 0;
      snprintf (uid_buf, sizeof uid_buf, "%d", caller_uid);
      argv[n++] = "udisks-helper-fstab-mounter";
      argv[n++] = "mount";
      argv[n++] = device->priv->device_file;
      argv[n++] = uid_buf;
      argv[n++] = NULL;
      goto run_job;
    }

  /* set the fstype */
  fstype = NULL;
  if (strlen (filesystem_type) == 0)
    {
      if (device->priv->id_type != NULL && strlen (device->priv->id_type) > 0)
        {
          fstype = g_strdup (device->priv->id_type);
        }
      else
        {
          fstype = g_strdup ("auto");
        }
    }
  else
    {
      fstype = g_strdup (filesystem_type);
    }

  fsmo = find_mount_options_for_fs (fstype);

  /* always prepend some reasonable default mount options; these are
   * chosen here; the user can override them if he wants to
   */
  options = prepend_default_mount_options (fsmo, caller_uid, given_options);

  /* validate mount options and check for authorizations */
  s = g_string_new ("uhelper=udisks,nodev,nosuid");
  for (n = 0; options[n] != NULL; n++)
    {
      const char *option = options[n];

      /* avoid attacks like passing "shortname=lower,uid=0" as a single mount option */
      if (strstr (option, ",") != NULL)
        {
          throw_error (context, ERROR_INVALID_OPTION, "Malformed mount option: ", option);
          g_string_free (s, TRUE);
          goto out;
        }

      /* first check if the mount option is allowed */
      if (!is_mount_option_allowed (fsmo, option, caller_uid))
        {
          throw_error (context, ERROR_INVALID_OPTION, "Mount option %s is not allowed", option);
          g_string_free (s, TRUE);
          goto out;
        }

      g_string_append_c (s, ',');
      g_string_append (s, option);
    }
  mount_options = g_string_free (s, FALSE);

  g_print ("**** USING MOUNT OPTIONS '%s' FOR DEVICE %s\n", mount_options, device->priv->device_file);

  if (device->priv->device_is_mounted)
    {
      throw_error (context, ERROR_FAILED, "Device is already mounted");
      goto out;
    }

  /* Determine the mount point to use.
   *
   * TODO: use characteristics of the drive such as the name, connection etc.
   *       to get better names (/media/disk is kinda lame).
   */
  if (device->priv->id_label != NULL && strlen (device->priv->id_label) > 0)
    {
      GString * s;

      s = g_string_new ("/media/");
      for (n = 0; device->priv->id_label[n] != '\0'; n++)
        {
          gint c = device->priv->id_label[n];
          if (c == '/')
            g_string_append_c (s, '_');
          else
            g_string_append_c (s, c);
        }

      mount_point = g_string_free (s, FALSE);
    }
  else if (device->priv->id_uuid != NULL && strlen (device->priv->id_uuid) > 0)
    {

      GString * s;

      s = g_string_new ("/media/");
      for (n = 0; device->priv->id_uuid[n] != '\0'; n++)
        {
          gint c = device->priv->id_uuid[n];
          if (c == '/')
            g_string_append_c (s, '_');
          else
            g_string_append_c (s, c);
        }

      mount_point = g_string_free (s, FALSE);

    }
  else
    {
      mount_point = g_strdup ("/media/disk");
    }

 try_another_mount_point:
  /* ... then uniqify the mount point and mkdir it */
  if (g_file_test (mount_point, G_FILE_TEST_EXISTS))
    {
      char *s = mount_point;
      /* TODO: append numbers instead of _, __ and so on */
      mount_point = g_strdup_printf ("%s_", mount_point);
      g_free (s);
      goto try_another_mount_point;
    }

  remove_dir_on_unmount = TRUE;

  if (g_mkdir (mount_point, 0700) != 0)
    {
      throw_error (context, ERROR_FAILED, "Error creating moint point: %m");
      goto out;
    }

  /* now that we have a mount point, immediately add it to the
   * /var/lib/udisks/mtab file.
   *
   * If mounting fails we'll clean it up in filesystem_mount_completed_cb. If it
   * hangs we'll clean it up the next time we start up.
   */
  mount_file_add (device->priv->device_file, mount_point, caller_uid, remove_dir_on_unmount);

  n = 0;
  argv[n++] = "mount";
  argv[n++] = "-t";
  argv[n++] = fstype;
  argv[n++] = "-o";
  argv[n++] = mount_options;
  argv[n++] = device->priv->device_file;
  argv[n++] = mount_point;
  argv[n++] = NULL;

 run_job:

  error = NULL;
  if (!job_new (context,
                "FilesystemMount",
                FALSE,
                device,
                argv,
                NULL,
                filesystem_mount_completed_cb,
                FALSE,
                filesystem_mount_data_new (mount_point, remove_dir_on_unmount),
                (GDestroyNotify) filesystem_mount_data_free))
    {
      if (remove_dir_on_unmount)
        {
          mount_file_remove (device->priv->device_file, mount_point);
          if (g_rmdir (mount_point) != 0)
            {
              g_warning ("Error removing dir in early mount error path: %m");
            }
        }
      goto out;
    }

 out:
  g_free (fstype);
  g_free (mount_options);
  g_free (mount_point);
  g_strfreev (options);
}
