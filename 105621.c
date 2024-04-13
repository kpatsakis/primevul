update_info_in_idle (Device *device)
{
  UpdateInfoInIdleData *data;

  data = g_new0 (UpdateInfoInIdleData, 1);
  data->device = device;
  data->idle_id = g_idle_add_full (G_PRIORITY_HIGH_IDLE,
                                   update_info_in_idle_cb,
                                   data,
                                   (GDestroyNotify) update_info_in_idle_data_free);

  g_object_weak_ref (G_OBJECT (device), update_info_in_idle_device_unreffed, data);
}
