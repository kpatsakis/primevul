update_info_in_idle_data_free (UpdateInfoInIdleData *data)
{
  g_object_weak_unref (G_OBJECT (data->device), update_info_in_idle_device_unreffed, data);
  g_free (data);
}
