_dbus_path_is_absolute (const DBusString *filename)
{
  if (_dbus_string_get_length (filename) > 0)
    return _dbus_string_get_byte (filename, 1) == ':'
           || _dbus_string_get_byte (filename, 0) == '\\'
           || _dbus_string_get_byte (filename, 0) == '/';
  else
    return FALSE;
}
