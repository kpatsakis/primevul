device_luks_change_passphrase_authorized_cb (Daemon *daemon,
                                             Device *device,
                                             DBusGMethodInvocation *context,
                                             const gchar *action_id,
                                             guint num_user_data,
                                             gpointer *user_data_elements)
{
  const char *old_secret = user_data_elements[0];
  const char *new_secret = user_data_elements[1];
  int n;
  char *argv[10];
  GError *error;
  char *secrets_as_stdin;

  secrets_as_stdin = NULL;

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "crypto") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a LUKS crypto device");
      goto out;
    }

  secrets_as_stdin = g_strdup_printf ("%s\n%s\n", old_secret, new_secret);

  n = 0;
  argv[n++] = "udisks-helper-change-luks-password";
  argv[n++] = device->priv->device_file;
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context,
                "LuksChangePassphrase",
                FALSE,
                device,
                argv,
                secrets_as_stdin,
                luks_change_passphrase_completed_cb,
                FALSE,
                NULL,
                NULL))
    {
      goto out;
    }

 out:
  /* scrub the secrets */
  if (secrets_as_stdin != NULL)
    {
      memset (secrets_as_stdin, '\0', strlen (secrets_as_stdin));
    }
  g_free (secrets_as_stdin);
}
