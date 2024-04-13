spindown_inhibitor_disconnected_cb (Inhibitor *inhibitor,
                                    Device *device)
{
  device->priv->spindown_inhibitors = g_list_remove (device->priv->spindown_inhibitors, inhibitor);
  g_signal_handlers_disconnect_by_func (inhibitor, spindown_inhibitor_disconnected_cb, device);
  g_object_unref (inhibitor);

  update_info (device);
  drain_pending_changes (device, FALSE);
  daemon_local_update_spindown (device->priv->daemon);
}
