tristring_from_message (DBusMessage *message)
{
  const char *path;
  const char *interface;

  path = dbus_message_get_path (message);
  interface = dbus_message_get_interface (message);

  g_assert (path);
  g_assert (interface);
  
  return tristring_alloc_from_strings (0,
                                       dbus_message_get_sender (message),
                                       path, interface);
}
