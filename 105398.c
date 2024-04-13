device_drive_ata_smart_initiate_selftest_authorized_cb (Daemon *daemon,
                                                        Device *device,
                                                        DBusGMethodInvocation *context,
                                                        const gchar *action_id,
                                                        guint num_user_data,
                                                        gpointer *user_data_elements)
{
  const gchar *test = user_data_elements[0];
  /* TODO: use options */
  int n;
  char *argv[10];
  GError *error;
  const gchar *job_name;

  if (g_strcmp0 (test, "short") == 0)
    {
      job_name = "DriveAtaSmartSelftestShort";
    }
  else if (g_strcmp0 (test, "extended") == 0)
    {
      job_name = "DriveAtaSmartSelftestExtended";
    }
  else if (g_strcmp0 (test, "conveyance") == 0)
    {
      job_name = "DriveAtaSmartSelftestConveyance";
    }
  else
    {
      throw_error (context, ERROR_FAILED, "Malformed test");
      goto out;
    }

  n = 0;
  argv[n++] = "udisks-helper-ata-smart-selftest";
  argv[n++] = device->priv->device_file;
  argv[n++] = (char *) test;
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context,
                job_name,
                TRUE,
                device,
                argv,
                NULL,
                drive_ata_smart_initiate_selftest_completed_cb,
                FALSE,
                NULL,
                NULL))
    {
      goto out;
    }

 out:
  ;
}
