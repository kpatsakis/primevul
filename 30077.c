_dbus_header_free (DBusHeader *header)
{
  _dbus_string_free (&header->data);
}
