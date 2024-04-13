simple_method_call (void)
{
  DBusMessage *message;
  /* Keeping this small makes stuff go faster */
  message = dbus_message_new_method_call ("o.b.Q",
                                          "/f/b",
                                          "o.b.Z",
                                          "Fro");
  if (message == NULL)
    _dbus_assert_not_reached ("oom");
  return message;
}
