poll_syncing_md_device (gpointer user_data)
{
  Device *device = DEVICE (user_data);

  g_print ("**** POLL SYNCING MD %s\n", device->priv->native_path);

  device->priv->linux_md_poll_timeout_id = 0;
  daemon_local_synthesize_changed (device->priv->daemon, device);
  return FALSE;
}
