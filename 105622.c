update_info_in_idle_cb (gpointer user_data)
{
  UpdateInfoInIdleData *data = user_data;

  /* this indirectly calls update_info and also removes the device
   * if it wants to be removed (e.g. if update_info() returns FALSE)
   */
  daemon_local_synthesize_changed (data->device->priv->daemon, data->device);

  return FALSE; /* remove source */
}
