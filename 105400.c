device_drive_ata_smart_refresh_data_authorized_cb (Daemon *daemon,
                                                   Device *device,
                                                   DBusGMethodInvocation *context,
                                                   const gchar *action_id,
                                                   guint num_user_data,
                                                   gpointer *user_data_elements)
{
  char **options = user_data_elements[0];
  int n;
  char *argv[10];
  GError *error;
  const char *simuldata;
  gboolean nowakeup;
  uid_t caller_uid;

  PROFILE ("device_drive_ata_smart_refresh_data_authorized_cb(device=%s) start", device->priv->native_path);
  daemon_local_get_uid (device->priv->daemon, &caller_uid, context);

  simuldata = NULL;
  nowakeup = FALSE;
  for (n = 0; options[n] != NULL; n++)
    {
      if (g_str_has_prefix (options[n], "simulate="))
        {
          if (context != NULL)
            {
              if (caller_uid != 0)
                {
                  throw_error (context, ERROR_FAILED, "Only uid 0 may use the simulate= option");
                  goto out;
                }
            }
          simuldata = (const char *) options[n] + 9;
          device_set_drive_ata_smart_is_available (device, TRUE);
        }
      else if (strcmp (options[n], "nowakeup") == 0)
        {
          nowakeup = TRUE;
        }
    }

  if (!device->priv->drive_ata_smart_is_available)
    {
      throw_error (context, ERROR_FAILED, "Device does not support ATA SMART");
      goto out;
    }

  if (simuldata != NULL)
    {
      n = 0;
      argv[n++] = "base64"; /* provided by coreutils */
      argv[n++] = (char *) simuldata;
      argv[n++] = NULL;
    }
  else
    {
      n = 0;
      argv[n++] = "udisks-helper-ata-smart-collect";
      argv[n++] = device->priv->device_file;
      argv[n++] = nowakeup ? "1" : "0";
      argv[n++] = NULL;
    }

  error = NULL;

  if (!job_new (context, NULL, /* don't run this as a job */
                FALSE, device, argv, NULL, drive_ata_smart_refresh_data_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
  PROFILE ("device_drive_ata_smart_refresh_data_authorized_cb(device=%s) end", device->priv->native_path);
}
