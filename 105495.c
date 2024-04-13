force_luks_teardown_cleartext_done (Device *device,
                                    gboolean success,
                                    gpointer user_data)
{
  int n;
  char *argv[16];
  ForceLuksTeardownData *data = user_data;

  if (!success)
    {
      if (data->fr_callback != NULL)
        data->fr_callback (data->device, FALSE, data->fr_user_data);

      force_luks_teardown_data_unref (data);
      goto out;
    }

  /* ok, clear text device is out of the way; now tear it down */

  n = 0;
  argv[n++] = "cryptsetup";
  argv[n++] = "luksClose";
  argv[n++] = data->dm_name;
  argv[n++] = NULL;


  if (!job_new (NULL,
                "ForceLuksTeardown",
                FALSE,
                data->device,
                argv,
                NULL,
                force_luks_teardown_completed_cb,
                FALSE,
                data,
                (GDestroyNotify) force_luks_teardown_data_unref))
    {

      g_warning ("Couldn't spawn cryptsetup for force teardown for device %s", data->dm_name);
      if (data->fr_callback != NULL)
        data->fr_callback (data->device, FALSE, data->fr_user_data);

      force_luks_teardown_data_unref (data);
    }
 out:
  ;
}
