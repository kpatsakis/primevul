generate_from_message (DBusString            *data,
                       DBusValidity          *expected_validity,
                       DBusMessage           *message)
{
  dbus_message_set_serial (message, 1);
  dbus_message_lock (message);

  *expected_validity = DBUS_VALID;
  
  /* move for efficiency, since we'll nuke the message anyway */
  if (!_dbus_string_move (&message->header.data, 0,
                          data, 0))
    _dbus_assert_not_reached ("oom");

  if (!_dbus_string_copy (&message->body, 0,
                          data, _dbus_string_get_length (data)))
    _dbus_assert_not_reached ("oom");
}
