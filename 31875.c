simple_signal (void)
{
  DBusMessage *message;
  message = dbus_message_new_signal ("/f/b",
                                     "o.b.Z",
                                     "Fro");
  if (message == NULL)
    _dbus_assert_not_reached ("oom");
  return message;
}
