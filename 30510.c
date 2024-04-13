_dbus_generate_pseudorandom_bytes (DBusString *str,
                                   int         n_bytes)
{
  int old_len;
  char *p;

  old_len = _dbus_string_get_length (str);

  if (!_dbus_string_lengthen (str, n_bytes))
    return FALSE;

  p = _dbus_string_get_data_len (str, old_len, n_bytes);

  _dbus_generate_pseudorandom_bytes_buffer (p, n_bytes);

  return TRUE;
}
