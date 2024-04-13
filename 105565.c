lvm2_lv_create_device_not_seen_cb (gpointer user_data)
{
  CreateLvm2LVData *data = user_data;

  throw_error (data->context,
               ERROR_FAILED,
               "Error creating Logical Volume: timeout (10s) waiting for LV to show up");

  g_signal_handler_disconnect (data->daemon, data->device_added_signal_handler_id);
  g_signal_handler_disconnect (data->daemon, data->device_changed_signal_handler_id);
  lvm2_lv_create_data_unref (data);

  return FALSE;
}
