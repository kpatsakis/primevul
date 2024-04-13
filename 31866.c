generate_outer (DBusMessageDataIter   *iter,
                DBusString            *data,
                DBusValidity          *expected_validity,
                DBusInnerGeneratorFunc func)
{
  DBusMessage *message;

  message = NULL;
  if (!(*func)(iter, &message))
    return FALSE;

  iter_next (iter);
  
  _dbus_assert (message != NULL);

  generate_from_message (data, expected_validity, message);

  dbus_message_unref (message);

  return TRUE;
}
