_dbus_header_update_lengths (DBusHeader *header,
                             int         body_len)
{
  _dbus_marshal_set_uint32 (&header->data,
                            BODY_LENGTH_OFFSET,
                            body_len,
                            header->byte_order);
}
