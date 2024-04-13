update_info_linux_dmmp_component (Device *device)
{
  gboolean is_dmmp_component;

  is_dmmp_component = FALSE;

  if (device->priv->holders_objpath->len == 1)
    {
      Device *holder;

      holder = daemon_local_find_by_object_path (device->priv->daemon, device->priv->holders_objpath->pdata[0]);
      if (holder != NULL && holder->priv->device_is_linux_dmmp)
        {
          is_dmmp_component = TRUE;
          device_set_linux_dmmp_component_holder (device, holder->priv->object_path);
        }
    }

  device_set_device_is_linux_dmmp_component (device, is_dmmp_component);
  return TRUE;
}
