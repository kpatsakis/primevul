get_dmmp_device_node (Device *device)
{
  static gchar buf[1024];

  g_assert (device->priv->device_is_linux_dmmp);
  g_snprintf (buf, sizeof (buf), "/dev/mapper/%s", device->priv->linux_dmmp_name);
  return buf;
}
