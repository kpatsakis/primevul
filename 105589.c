partition_table_create_device_not_changed_cb (gpointer user_data)
{
  CreatePartitionTableData *data = user_data;

  throw_error (data->context, ERROR_FAILED, "Error creating partition table: timeout (10s) waiting for change");

  g_signal_handler_disconnect (data->device->priv->daemon, data->device_changed_signal_handler_id);
  partition_table_create_data_unref (data);

  return FALSE;
}
