device_filesystem_create_authorized_cb (Daemon *daemon,
                                        Device *device,
                                        DBusGMethodInvocation *context,
                                        const gchar *action_id,
                                        guint num_user_data,
                                        gpointer *user_data_elements)
{
  const gchar *fstype = user_data_elements[0];
  gchar **options = user_data_elements[1];
  device_filesystem_create_internal (device, fstype, options, NULL, NULL, context);
}
