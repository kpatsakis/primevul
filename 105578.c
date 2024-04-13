partition_create_filesystem_create_hook (DBusGMethodInvocation *context,
                                         Device *device,
                                         gboolean filesystem_create_succeeded,
                                         gpointer user_data)
{
  if (!filesystem_create_succeeded)
    {
      /* dang.. FilesystemCreate already reported an error */
    }
  else
    {
      /* it worked.. */
      dbus_g_method_return (context, device->priv->object_path);
    }
}
