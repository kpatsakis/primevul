device_luks_lock_authorized_cb (Daemon *daemon,
                                Device *device,
                                DBusGMethodInvocation *context,
                                const gchar *action_id,
                                guint num_user_data,
                                gpointer *user_data_elements)
{
  /* TODO: use options */
  Device *cleartext_device;
  int n;
  char *argv[10];
  GError *error;

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "crypto") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a LUKS crypto device");
      goto out;
    }

  cleartext_device = find_cleartext_device (device);
  if (cleartext_device == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cleartext device is not unlocked");
      goto out;
    }

  if (cleartext_device->priv->dm_name == NULL || strlen (cleartext_device->priv->dm_name) == 0)
    {
      throw_error (context, ERROR_FAILED, "Cannot determine device-mapper name");
      goto out;
    }

  n = 0;
  argv[n++] = "cryptsetup";
  argv[n++] = "luksClose";
  argv[n++] = cleartext_device->priv->dm_name;
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context,
                "LuksLock",
                FALSE,
                device,
                argv,
                NULL,
                luks_lock_completed_cb,
                FALSE,
                lock_encryption_data_new (context, device, cleartext_device),
                (GDestroyNotify) lock_encryption_data_unref))
    {
      goto out;
    }

 out:
  ;
}
