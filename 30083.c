_dbus_header_reinit (DBusHeader *header,
                     int         byte_order)
{
  _dbus_string_set_length (&header->data, 0);

  header->byte_order = byte_order;
  header->padding = 0;

  _dbus_header_cache_invalidate_all (header);
}
