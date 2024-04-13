_dbus_header_copy (const DBusHeader *header,
                   DBusHeader       *dest)
{
  *dest = *header;

  if (!_dbus_string_init_preallocated (&dest->data,
                                       _dbus_string_get_length (&header->data)))
    return FALSE;

  if (!_dbus_string_copy (&header->data, 0, &dest->data, 0))
    {
      _dbus_string_free (&dest->data);
      return FALSE;
    }

  /* Reset the serial */
  _dbus_header_set_serial (dest, 0);

  return TRUE;
}
