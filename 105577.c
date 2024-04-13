partition_create_device_not_seen_cb (gpointer user_data)
{
  CreatePartitionData *data = user_data;

  throw_error (data->context, ERROR_FAILED, "Error creating partition: timeout (10s) waiting for partition to show up");

  g_signal_handler_disconnect (data->device->priv->daemon, data->device_added_signal_handler_id);
  partition_create_data_unref (data);

  return FALSE;
}
