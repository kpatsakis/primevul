device_filesystem_create_internal (Device *device,
                                   const char *fstype,
                                   char **options,
                                   FilesystemCreateHookFunc hook_func,
                                   gpointer hook_user_data,
                                   DBusGMethodInvocation *context)
{
  int n, m;
  char *argv[128];
  GError *error;
  char *s;
  char *options_for_stdin;
  char *passphrase_stdin;
  MkfsData *mkfs_data;

  options_for_stdin = NULL;
  passphrase_stdin = NULL;
  error = NULL;

  if (device_local_is_busy (device, TRUE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  if (strlen (fstype) == 0)
    {
      throw_error (context, ERROR_FAILED, "fstype not specified");
      goto out;
    }

  /* search for luks_encrypt=<passphrase> and do a detour if that's specified */
  for (n = 0; options[n] != NULL; n++)
    {
      if (g_str_has_prefix (options[n], "luks_encrypt="))
        {
          MkfsLuksData *mkfse_data;

          /* So this is a request to create an luks device to put the
           * file system on; save all options for mkfs (except luks_encrypt=) for
           * later invocation once we have a cleartext device.
           */

          mkfse_data = g_new0 (MkfsLuksData, 1);
          mkfse_data->refcount = 1;
          mkfse_data->context = context;
          mkfse_data->device = g_object_ref (device);
          mkfse_data->passphrase = g_strdup (options[n] + sizeof("luks_encrypt=") - 1);
          mkfse_data->mkfs_hook_func = hook_func;
          mkfse_data->mkfs_hook_user_data = hook_user_data;
          mkfse_data->fstype = g_strdup (fstype);
          mkfse_data->options = g_strdupv (options);
          g_free (mkfse_data->options[n]);
          for (m = n; mkfse_data->options[m] != NULL; m++)
            {
              mkfse_data->options[m] = mkfse_data->options[m + 1];
            }

          passphrase_stdin = g_strdup_printf ("%s\n", mkfse_data->passphrase);

          n = 0;
          argv[n++] = "cryptsetup";
          argv[n++] = "-q";
          argv[n++] = "luksFormat";
          argv[n++] = device->priv->device_file;
          argv[n++] = NULL;

          error = NULL;
          if (!job_new (context,
                        "LuksFormat",
                        TRUE,
                        device,
                        argv,
                        passphrase_stdin,
                        filesystem_create_create_luks_device_completed_cb,
                        FALSE,
                        mkfse_data,
                        (GDestroyNotify) mkfse_data_unref))
            {
              goto out;
            }

          goto out;
        }
    }

  mkfs_data = g_new (MkfsData, 1);
  mkfs_data->hook_func = hook_func;
  mkfs_data->hook_user_data = hook_user_data;

  /* pass options on stdin as it may contain secrets */
  s = g_strjoinv ("\n", options);
  options_for_stdin = g_strconcat (s, "\n\n", NULL);
  g_free (s);

  n = 0;
  argv[n++] = "udisks-helper-mkfs";
  argv[n++] = (char *) fstype;
  argv[n++] = device->priv->device_file;
  argv[n++] = device->priv->device_is_partition_table ? "1" : "0";
  argv[n++] = NULL;

  if (!job_new (context,
                "FilesystemCreate",
                TRUE,
                device,
                argv,
                options_for_stdin,
                filesystem_create_completed_cb,
                FALSE,
                mkfs_data,
                (GDestroyNotify) mkfs_data_unref))
    {
      goto out;
    }

 out:
  g_free (options_for_stdin);
  if (passphrase_stdin != NULL)
    {
      memset (passphrase_stdin, '\0', strlen (passphrase_stdin));
      g_free (passphrase_stdin);
    }
  return TRUE;
}
