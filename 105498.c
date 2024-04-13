force_luks_teardown_data_unref (ForceLuksTeardownData *data)
{
  if (data->device != NULL)
    g_object_unref (data->device);
  g_free (data->dm_name);
  g_free (data);
}
