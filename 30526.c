_dbus_parse_uid (const DBusString      *uid_str,
                 dbus_uid_t            *uid)
{
  int end;
  long val;

  if (_dbus_string_get_length (uid_str) == 0)
    {
      _dbus_verbose ("UID string was zero length\n");
      return FALSE;
    }

  val = -1;
  end = 0;
  if (!_dbus_string_parse_int (uid_str, 0, &val,
                               &end))
    {
      _dbus_verbose ("could not parse string as a UID\n");
      return FALSE;
    }

  if (end != _dbus_string_get_length (uid_str))
    {
      _dbus_verbose ("string contained trailing stuff after UID\n");
      return FALSE;
    }

  *uid = val;

  return TRUE;
}
