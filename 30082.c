_dbus_header_init (DBusHeader *header,
                   int         byte_order)
{
  if (!_dbus_string_init_preallocated (&header->data, 32))
    return FALSE;

  _dbus_header_reinit (header, byte_order);

  return TRUE;
}
