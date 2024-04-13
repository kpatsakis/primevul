force_luks_teardown_data_new (Device *device,
                              const char *dm_name,
                              ForceRemovalCompleteFunc fr_callback,
                              gpointer fr_user_data)
{
  ForceLuksTeardownData *data;

  data = g_new0 (ForceLuksTeardownData, 1);
  data->device = g_object_ref (device);
  data->dm_name = g_strdup (dm_name);
  data->fr_callback = fr_callback;
  data->fr_user_data = fr_user_data;
  return data;
}
