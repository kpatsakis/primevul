linux_md_remove_component_device_not_seen_cb (gpointer user_data)
{
  RemoveComponentData *data = user_data;

  throw_error (data->context,
               ERROR_FAILED,
               "Error removing component: timeout (10s) waiting for slave to stop being busy");

  g_signal_handler_disconnect (data->slave->priv->daemon, data->device_changed_signal_handler_id);
  remove_component_data_unref (data);

  return FALSE;
}
