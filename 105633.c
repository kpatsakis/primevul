update_info_luks (Device *device)
{
  if (g_strcmp0 (device->priv->id_type, "crypto_LUKS") == 0 && device->priv->holders_objpath->len == 1)
    {
      device_set_device_is_luks (device, TRUE);
      device_set_luks_holder (device, device->priv->holders_objpath->pdata[0]);
    }
  else
    {
      device_set_device_is_luks (device, FALSE);
      device_set_luks_holder (device, NULL);
    }

  return TRUE;
}
