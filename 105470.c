drive_ata_smart_refresh_data_completed_cb (DBusGMethodInvocation *context,
                                           Device *device,
                                           gboolean job_was_cancelled,
                                           int status,
                                           const char *stderr,
                                           const char *stdout,
                                           gpointer user_data)
{
  gint rc;
  SkDisk *d;
  gchar *blob;
  gsize blob_size;
  time_t time_collected;
  SkSmartOverall overall;

  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) start", device->priv->native_path);

  d = NULL;
  blob = NULL;

  if (job_was_cancelled || stdout == NULL)
    {
      if (job_was_cancelled)
        {
          if (context != NULL)
            throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else
        {
          if (context != NULL)
            throw_error (context,
                         ERROR_FAILED,
                         "Error retrieving ATA SMART data: no output",
                         WEXITSTATUS (status),
                         stderr);
        }
      goto out;
    }

  rc = WEXITSTATUS (status);

  if (rc != 0)
    {
      if (rc == 2)
        {
          if (context != NULL)
            {
              throw_error (context, ERROR_ATA_SMART_WOULD_WAKEUP, "Error retrieving ATA SMART data: %s", stderr);
            }
        }
      else
        {
          if (context != NULL)
            {
              throw_error (context,
                           ERROR_FAILED,
                           "Error retrieving ATA SMART data: helper failed with exit code %d: %s",
                           rc,
                           stderr);
            }
        }
      goto out;
    }

  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) decode blob", device->priv->native_path);
  blob = (gchar *) g_base64_decode (stdout, &blob_size);

  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) set blob", device->priv->native_path);
  if (blob == NULL)
    {
      if (context != NULL)
        {
          throw_error (context, ERROR_FAILED, "Error decoding ATA SMART data: invalid base64 format: %s", stdout);
        }
      else
        {
          g_warning ("Error decoding ATA SMART data: invalid base64 format: %s", stdout);
        }
      goto out;
    }

  if (sk_disk_open (NULL, &d) != 0)
    {
      if (context != NULL)
        {
          throw_error (context, ERROR_FAILED, "unable to open a SkDisk");
        }
      goto out;
    }

  if (sk_disk_set_blob (d, blob, blob_size) != 0)
    {
      if (context != NULL)
        {
          throw_error (context, ERROR_FAILED, "error parsing blob: %s", strerror (errno));
        }
      goto out;
    }

  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) time collected", device->priv->native_path);
  time_collected = time (NULL);
  device_set_drive_ata_smart_time_collected (device, time_collected);

  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) overall smart status", device->priv->native_path);
  if (sk_disk_smart_get_overall (d, &overall) != 0)
    overall = -1;
  device_set_drive_ata_smart_status (device, overall);
  device_set_drive_ata_smart_blob_steal (device, blob, blob_size);
  blob = NULL;

  /* emit change event since we've updated the smart data */
  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) drain pending changes", device->priv->native_path);
  drain_pending_changes (device, FALSE);

  if (context != NULL)
    dbus_g_method_return (context);

 out:
  g_free (blob);
  if (d != NULL)
    sk_disk_free (d);
  PROFILE ("drive_ata_smart_refresh_data_completed_cb(device=%s) end", device->priv->native_path);
}
