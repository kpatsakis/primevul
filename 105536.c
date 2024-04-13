linux_md_create_device_not_seen_cb (gpointer user_data)
{
  LinuxMdCreateData *data = user_data;

  throw_error (data->context, ERROR_FAILED, "Error assembling array: timeout (10s) waiting for array to show up");

  g_signal_handler_disconnect (data->daemon, data->device_added_signal_handler_id);
  linux_md_create_data_unref (data);
  return FALSE;
}
