_dbus_header_get_serial (DBusHeader *header)
{
  return _dbus_marshal_read_uint32 (&header->data,
                                    SERIAL_OFFSET,
                                    header->byte_order,
                                    NULL);
}
